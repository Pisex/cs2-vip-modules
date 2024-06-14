#include <stdio.h>
#include "vip_regen_armor.h"
#include <fstream>
#include "entitykeyvalues.h"
#include <sstream>

vip_regen_armor g_vip_regen_armor;

IUtilsApi* g_pUtils;
IMenusApi* g_pMenus;
IVIPApi* g_pVIPCore;

float g_flUniversalTime;
float g_flLastTickedTime;
bool g_bHasTicked;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(vip_regen_armor, g_vip_regen_armor);

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

bool RegenArmor(int iSlot)
{
	auto pController = CCSPlayerController::FromSlot(iSlot);
	if (!pController)
		return false;
	if(pController->m_iTeamNum() < 2 || !pController->IsAlive())
		return false;

	int iArmor = pController->GetPlayerPawn()->m_ArmorValue();
	const char* sArmor = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "armor");
	int iMaxArmor = 100;
	if(sArmor[0])
	{
		if(strlen(sArmor) > 0)
		{
			if(sArmor[0] == '+')
			{
				std::string str(sArmor);
				str.erase(0,2);
				iMaxArmor = std::stoi(str);
			}
			else iMaxArmor = std::stoi(sArmor);
		}
	}
	if(iArmor < iMaxArmor)
	{
		int iNewArmor = iArmor + g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "RegenArmor");
		if(iNewArmor < iMaxArmor)
		{
			pController->GetPlayerPawn()->m_ArmorValue(iNewArmor);
			return true;
		}

		pController->GetPlayerPawn()->m_ArmorValue(iMaxArmor);
	}

	g_bRegen[iSlot] = false;

	return false;
}

bool vip_regen_armor::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetServerFactory, g_pSource2GameClients, IServerGameClients, SOURCE2GAMECLIENTS_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	ConVar_Register(FCVAR_RELEASE | FCVAR_CLIENT_CAN_EXECUTE | FCVAR_GAMEDLL);

	SH_ADD_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_regen_armor::GameFrame), true);

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
				if(RegenArmor(i))
					g_iClientRegenTicks[i] = g_pVIPCore->VIP_GetClientFeatureInt(i, "IntervalRegenArmor");
			}
		}
		return 1.0f;
	});

	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_regen_armor::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_regen_armor::GameFrame), true);
	return true;
}

void vip_regen_armor::GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
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
		
		if( g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "RegenArmor") == 0 || pEvent->GetInt("dmg_armor") == 0)
			return;
		g_bRegen[iSlot] = true;
		g_iClientDelayTicks[iSlot] = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "DelayRegenArmor");
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
	int iRegenArmor = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "RegenArmor");
	int iCount = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "IntervalRegenArmor");
	char szDisplay[128];
	g_SMAPI->Format(szDisplay, sizeof(szDisplay), "%s [%i/%i s.]", g_pVIPCore->VIP_GetTranslate(szFeature), iRegenArmor, iCount);
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

void vip_regen_armor::AllPluginsLoaded()
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
	g_pVIPCore->VIP_RegisterFeature("RegenArmor", VIP_BOOL, TOGGLABLE, nullptr, OnToggle, OnDisplay);
	g_pVIPCore->VIP_RegisterFeature("DelayRegenArmor", VIP_INT, HIDE);
	g_pVIPCore->VIP_RegisterFeature("IntervalRegenArmor", VIP_INT, HIDE);
	g_pVIPCore->VIP_OnClientLoaded(OnClientLoad);
	g_pVIPCore->VIP_OnClientDisconnect(OnClientDisconnect);
	g_pVIPCore->VIP_OnPlayerSpawn(OnPlayerSpawn);
}

const char *vip_regen_armor::GetLicense()
{
	return "Public";
}

const char *vip_regen_armor::GetVersion()
{
	return "1.0";
}

const char *vip_regen_armor::GetDate()
{
	return __DATE__;
}

const char *vip_regen_armor::GetLogTag()
{
	return "[RA]";
}

const char *vip_regen_armor::GetAuthor()
{
	return "Pisex";
}

const char *vip_regen_armor::GetDescription()
{
	return "";
}

const char *vip_regen_armor::GetName()
{
	return "[VIP] Regen Armor";
}

const char *vip_regen_armor::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
