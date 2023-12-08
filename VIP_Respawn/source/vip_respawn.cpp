#include <stdio.h>
#include "vip_respawn.h"

vip_respawn g_vip_respawn;

IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

void (*UTIL_RespawnPlayer)(CBasePlayerPawn* pPlayer) = nullptr;

int g_iRespawns[63];
bool g_isActive;

PLUGIN_EXPOSE(vip_respawn, g_vip_respawn);

void OnRespawnCommand(const char* szContent, int iSlot)
{
	if(g_pVIPCore->VIP_IsClientVIP(iSlot))
	{
		int iCount = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "respawn");
		if(iCount > -1)
		{
			if(iCount > g_iRespawns[iSlot] || iCount == 0)
			{
				CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(iSlot + 1));
				if(!pPlayerController) return;
				CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
				if (!pPlayerPawn || pPlayerPawn->m_lifeState() == LIFE_ALIVE)
				{
					g_pVIPCore->VIP_PrintToChat(iSlot, 3, g_pVIPCore->VIP_GetTranslate("YourAlive"));
					return;
				}
				int iTeam = pPlayerController->m_iTeamNum();
				if(iTeam < 2)
				{
					g_pVIPCore->VIP_PrintToChat(iSlot, 3, g_pVIPCore->VIP_GetTranslate("SelectTeam"));
					return;
				}
				if(!g_isActive)
				{
					g_pVIPCore->VIP_PrintToChat(iSlot, 3, g_pVIPCore->VIP_GetTranslate("EndRound"));
					return;
				}
				g_iRespawns[iSlot]++;
				pPlayerController->ForceRespawn();
				UTIL_RespawnPlayer(pPlayerController->m_hPlayerPawn().Get());
			}
			else g_pVIPCore->VIP_PrintToChat(iSlot, 3, g_pVIPCore->VIP_GetTranslate("LimitRespawn"));
		}
	}
}

bool vip_respawn::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );

	CModule libserver(g_pSource2Server);
	UTIL_RespawnPlayer = libserver.FindPatternSIMD("8B 8F 40 0E 00 00 83 F9 FF 0F 84 D9 01").RCast< decltype(UTIL_RespawnPlayer) >();
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
	return true;
}

void VIP_OnFireEvent(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	if(!strcmp(szName, "round_start"))
	{
		for (size_t i = 0; i < 64; i++) g_iRespawns[i] = 0;
		g_isActive = true;
	}
	else if(!strcmp(szName, "round_end"))
	{
		g_isActive = false;
	}
}

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = g_pVIPCore->VIP_GetEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pVIPCore->VIP_OnFireEvent(VIP_OnFireEvent);
	g_pVIPCore->VIP_RegCommand("respawn", OnRespawnCommand);
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
	g_pVIPCore->VIP_OnVIPLoaded(VIP_OnVIPLoaded);
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
