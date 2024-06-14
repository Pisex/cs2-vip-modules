#include <stdio.h>
#include "vip_regen_hp.h"
#include <fstream>
#include "entitykeyvalues.h"
#include <sstream>

vip_regen_hp g_vip_regen_hp;

IUtilsApi* g_pUtils;
IMenusApi* g_pMenus;
IVIPApi* g_pVIPCore;

float g_flUniversalTime;
float g_flLastTickedTime;
bool g_bHasTicked;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(vip_regen_hp, g_vip_regen_hp);

SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);

bool g_bRegen[64];
int g_iClientDelayTicks[64],
	g_iClientRegenTicks[64];

CGameEntitySystem* GameEntitySystem()
{
	return g_pUtils->GetCGameEntitySystem();
};

void StartupServer()
{
	g_bHasTicked = false;
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pUtils->GetCEntitySystem();
}

bool RegenHP(int iSlot)
{
	auto pController = CCSPlayerController::FromSlot(iSlot);
	if (!pController)
		return false;
	if(pController->m_iTeamNum() < 2 || !pController->IsAlive())
		return false;
	int iMaxHP = pController->GetPlayerPawn()->m_iMaxHealth();
	int iHP = pController->GetPlayerPawn()->m_iHealth();
	if(iHP < iMaxHP)
	{
		iHP += g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "RegenHP");
		if(iHP < iMaxHP)
		{
			pController->GetPlayerPawn()->m_iHealth(iHP);
			return true;
		}
		pController->GetPlayerPawn()->m_iHealth(iMaxHP);
	}
	g_bRegen[iSlot] = false;

	return false;
}

bool vip_regen_hp::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetServerFactory, g_pSource2GameClients, IServerGameClients, SOURCE2GAMECLIENTS_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	ConVar_Register(FCVAR_RELEASE | FCVAR_CLIENT_CAN_EXECUTE | FCVAR_GAMEDLL);

	SH_ADD_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_regen_hp::GameFrame), true);

	new CTimer(1.0f, []() {
		for(int i = 0; i < 64; i++)
		{
			if(g_bRegen[i])
			{
				CCSPlayerController* pController = CCSPlayerController::FromSlot(i);
				if(!pController)
					continue;
				if(pController->m_iTeamNum() < 2 || !pController->IsAlive())
					continue;
				if(g_iClientDelayTicks[i] > 0)
				{
					g_iClientDelayTicks[i]--;
					continue;
				}
				if(g_iClientRegenTicks[i] > 0)
				{
					g_iClientRegenTicks[i]--;
					continue;
				}
				if(RegenHP(i))
					g_iClientRegenTicks[i] = g_pVIPCore->VIP_GetClientFeatureInt(i, "IntervalRegenHP");
			}
		}
		return 1.0f;
	});

	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_regen_hp::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_regen_hp::GameFrame), true);
	return true;
}

void vip_regen_hp::GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	if (simulating && g_bHasTicked)
	{
		g_flUniversalTime += g_pUtils->GetCGlobalVars()->curtime - g_flLastTickedTime;
	}

	g_flLastTickedTime = g_pUtils->GetCGlobalVars()->curtime;
	g_bHasTicked = true;

	for (int i = g_timers.Tail(); i != g_timers.InvalidIndex();)
	{
		auto timer = g_timers[i];

		int prevIndex = i;
		i = g_timers.Previous(i);

		if (timer->m_flLastExecute == -1)
			timer->m_flLastExecute = g_flUniversalTime;

		// Timer execute 
		if (timer->m_flLastExecute + timer->m_flInterval <= g_flUniversalTime)
		{
			if (!timer->Execute())
			{
				delete timer;
				g_timers.Remove(prevIndex);
			}
			else
			{
				timer->m_flLastExecute = g_flUniversalTime;
			}
		}
	}
}

void OnPlayerHurt(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	int iSlot = pEvent->GetInt("userid");
	if(iSlot < 0 || iSlot > 64)
		return;
	if(g_pVIPCore->VIP_IsClientVIP(iSlot))
	{
		auto pController = CCSPlayerController::FromSlot(iSlot);
		if (!pController)
			return;
		if(pController->m_iTeamNum() < 2 || !pController->IsAlive())
			return;
		
		if( g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "RegenHP") == 0 )
			return;
		g_bRegen[iSlot] = true;
		g_iClientDelayTicks[iSlot] = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "DelayRegenHP");
	}
}

void OnClientLoad(int iSlot, bool bIsVIP)
{
	g_bRegen[iSlot] = false;
}

void OnClientDisconnect(int iSlot, bool bIsVIP)
{
	g_bRegen[iSlot] = false;
}

void OnPlayerSpawn(int iSlot, int iTeam, bool bIsVIP)
{
	g_bRegen[iSlot] = false;
}

std::string OnDisplay(int iSlot, const char* szFeature)
{
	int iRegenHP = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "RegenHP");
	int iCount = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "IntervalRegenHP");
	char szDisplay[128];
	g_SMAPI->Format(szDisplay, sizeof(szDisplay), "%s [%i HP/%i s.]", g_pVIPCore->VIP_GetTranslate(szFeature), iRegenHP, iCount);
	return std::string(szDisplay);
}

bool OnToggle(int iSlot, const char* szFeature, VIP_ToggleState eOldStatus, VIP_ToggleState& eNewStatus)
{
	if(eNewStatus == DISABLED)
	{
		g_bRegen[iSlot] = false;
	}
	return true;

}

void vip_regen_hp::AllPluginsLoaded()
{
	char error[64];
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
		V_strncpy(error, "Failed to lookup menus core. Aborting", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pUtils->StartupServer(g_PLID, StartupServer);
	g_pUtils->HookEvent(g_PLID, "player_hurt", OnPlayerHurt);
	g_pVIPCore->VIP_RegisterFeature("RegenHP", VIP_BOOL, TOGGLABLE, nullptr, OnToggle, OnDisplay);
	g_pVIPCore->VIP_RegisterFeature("DelayRegenHP", VIP_INT, HIDE);
	g_pVIPCore->VIP_RegisterFeature("IntervalRegenHP", VIP_INT, HIDE);
	g_pVIPCore->VIP_OnClientLoaded(OnClientLoad);
	g_pVIPCore->VIP_OnClientDisconnect(OnClientDisconnect);
	g_pVIPCore->VIP_OnPlayerSpawn(OnPlayerSpawn);
}

const char *vip_regen_hp::GetLicense()
{
	return "Public";
}

const char *vip_regen_hp::GetVersion()
{
	return "1.0";
}

const char *vip_regen_hp::GetDate()
{
	return __DATE__;
}

const char *vip_regen_hp::GetLogTag()
{
	return "[RH]";
}

const char *vip_regen_hp::GetAuthor()
{
	return "Pisex";
}

const char *vip_regen_hp::GetDescription()
{
	return "";
}

const char *vip_regen_hp::GetName()
{
	return "[VIP] Regen HP";
}

const char *vip_regen_hp::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
