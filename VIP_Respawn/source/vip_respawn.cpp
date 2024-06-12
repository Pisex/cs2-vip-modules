#include <stdio.h>
#include "vip_respawn.h"

vip_respawn g_vip_respawn;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

void (*UTIL_RespawnPlayer)(CBasePlayerController* pPlayer, CEntityInstance* pPawn, bool a3, bool a4) = nullptr;

int g_iRespawns[63];
bool g_isActive;

PLUGIN_EXPOSE(vip_respawn, g_vip_respawn);


bool OnRespawnCommand(int iSlot, const char* szContent)
{
	if(g_pVIPCore->VIP_IsClientVIP(iSlot))
	{
		int iCount = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "respawn");
		if(iCount > -1)
		{
			if(iCount > g_iRespawns[iSlot] || iCount == 0)
			{
				CCSPlayerController* pPlayerController =  CCSPlayerController::FromSlot(iSlot);
				if(!pPlayerController) return false;
				CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
				if (!pPlayerPawn || pPlayerPawn->m_lifeState() == LIFE_ALIVE)
				{
					g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("YourAlive"));
					return false;
				}
				int iTeam = pPlayerController->m_iTeamNum();
				if(iTeam < 2)
				{
					g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("SelectTeam"));
					return false;
				}
				if(!g_isActive)
				{
					g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("EndRound"));
					return false;
				}
				g_iRespawns[iSlot]++;
				
				const auto currentPawn = pPlayerController->m_hPawn().Get();
				const auto playerPawn  = pPlayerController->m_hPlayerPawn().Get();
				if (currentPawn != playerPawn)
				{
					const auto pawn = UTIL_FindEntityByEHandle(playerPawn);
					if (pawn)
						UTIL_RespawnPlayer(pPlayerController, pawn, true, false);
				}
				pPlayerController->Respawn();
			}
			else g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("LimitRespawn"));
		}
	}
	return false;
}

bool OnSelect(int iSlot, const char* szFeature)
{
	OnRespawnCommand(iSlot, "");
	return false;
}

bool vip_respawn::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );

	DynLibUtils::CModule libserver(g_pSource2Server);
	UTIL_RespawnPlayer = libserver.FindPattern("55 48 89 E5 41 57 41 56 41 55 49 89 FD 41 54 45 89 C4").RCast< decltype(UTIL_RespawnPlayer) >();
	if (!UTIL_RespawnPlayer)
	{
		V_strncpy(error, "Failed to find function to get UTIL_RespawnPlayer", maxlen);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		return false;
	}
	return true;
}

bool vip_respawn::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	g_pUtils->ClearAllHooks(g_PLID);
	delete g_pUtils;
	return true;
}

void OnRoundStart(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	for (size_t i = 0; i < 64; i++) g_iRespawns[i] = 0;
	g_isActive = true;
}

void OnRoundEnd(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	g_isActive = false;
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pVIPCore->VIP_GetEntitySystem();
};

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pUtils->HookEvent(g_PLID, "round_start", OnRoundStart);
	g_pUtils->HookEvent(g_PLID, "round_end", OnRoundEnd);
	g_pUtils->RegCommand(g_PLID, {"mm_respawn", "sm_respawn", "respawn"}, {"!respawn", "respawn"}, OnRespawnCommand);
}

std::string OnDisplay(int iSlot, const char* szFeature)
{
	int iCount = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "respawn");
	char szDisplay[128];
	g_SMAPI->Format(szDisplay, sizeof(szDisplay), "%s [%i]", g_pVIPCore->VIP_GetTranslate(szFeature), iCount - g_iRespawns[iSlot]);
	return std::string(szDisplay);
}

void vip_respawn::AllPluginsLoaded()
{
	int ret;
	g_pVIPCore = (IVIPApi*)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);

	if (ret == META_IFACE_FAILED)
	{
		char error[64];
		V_strncpy(error, "Failed to lookup vip core. Aborting", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pUtils = (IUtilsApi*)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);

	if (ret == META_IFACE_FAILED)
	{
		char error[64];
		V_strncpy(error, "Failed to lookup utils api. Aborting", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pVIPCore->VIP_OnVIPLoaded(VIP_OnVIPLoaded);
	g_pVIPCore->VIP_RegisterFeature("respawn", VIP_INT, SELECTABLE, OnSelect, nullptr, OnDisplay);
}

const char *vip_respawn::GetLicense()
{
	return "Public";
}

const char *vip_respawn::GetVersion()
{
	return "1.0";
}

const char *vip_respawn::GetDate()
{
	return __DATE__;
}

const char *vip_respawn::GetLogTag()
{
	return "[VIP-RESPAWN]";
}

const char *vip_respawn::GetAuthor()
{
	return "Pisex";
}

const char *vip_respawn::GetDescription()
{
	return "";
}

const char *vip_respawn::GetName()
{
	return "[VIP] Respawn";
}

const char *vip_respawn::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
