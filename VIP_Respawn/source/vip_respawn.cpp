#include <stdio.h>
#include "vip_respawn.h"

vip_respawn g_vip_respawn;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

void (*UTIL_RespawnPlayer2)(CBasePlayerController* pPlayer, CEntityInstance* pPawn, bool a3, bool a4) = nullptr;
void (*UTIL_RespawnPlayer)(CBasePlayerPawn* pPlayer) = nullptr;

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
				CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(iSlot + 1));
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
						UTIL_RespawnPlayer2(pPlayerController, pawn, true, false);
				}
				pPlayerController->ForceRespawn();
				UTIL_RespawnPlayer(pPlayerController->m_hPlayerPawn().Get());
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
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );

	CModule libserver(g_pSource2Server);
	UTIL_RespawnPlayer2 = libserver.FindPatternSIMD(WIN_LINUX("44 88 4C 24 20 55 57", "55 48 89 E5 41 57 41 56 41 55 41 54 49 89 FC 53 48 89 F3 48 81 EC C8 00 00 00")).RCast< decltype(UTIL_RespawnPlayer2) >();
	if (!UTIL_RespawnPlayer2)
	{
		V_strncpy(error, "Failed to find function to get UTIL_RespawnPlayer2", maxlen);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		return false;
	}
	UTIL_RespawnPlayer = libserver.FindPatternSIMD(WIN_LINUX("40 53 48 83 EC 20 8B 91 38 0B 00 00 48 8B D9", "8B 8F 40 0E 00 00 83 F9 FF 0F 84 D9 01")).RCast< decltype(UTIL_RespawnPlayer) >();
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

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = g_pVIPCore->VIP_GetEntitySystem();
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
	g_pVIPCore->VIP_RegisterFeature("respawn", INT, SELECTABLE, OnSelect, nullptr, OnDisplay);
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
