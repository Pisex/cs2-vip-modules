#include <stdio.h>
#include "vip_ks.h"

VIP_KS g_VIP_KS;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;
CGlobalVars* gpGlobals = nullptr;

float fTime;

PLUGIN_EXPOSE(VIP_KS, g_VIP_KS);
bool VIP_KS::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );

	{
		KeyValues* hKv = new KeyValues("VIP");
		const char *pszPath = "addons/configs/vip/vip_ks.ini";

		if (!hKv->LoadFromFile(g_pFullFileSystem, pszPath))
		{
			Warning("Failed to load %s\n", pszPath);
			return false;
		}

		fTime = hKv->GetFloat("effect_time");
		delete hKv;
	}
	return true;
}

bool VIP_KS::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	g_pUtils->ClearAllHooks(g_PLID);
	delete g_pUtils;
	return true;
}

void OnStartupServer()
{
	gpGlobals = g_pUtils->GetCGlobalVars();
	g_pGameEntitySystem = g_pUtils->GetCGameEntitySystem();
	g_pEntitySystem = g_pUtils->GetCEntitySystem();
}

void OnPlayerDeath(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	CCSPlayerController* pPlayerController = static_cast<CCSPlayerController*>(pEvent->GetPlayerController("attacker"));
    if(pPlayerController && pEvent->GetInt("attacker") != pEvent->GetInt("userid") && g_pVIPCore->VIP_GetClientFeatureBool(pEvent->GetInt("attacker"), "killscreen"))
	{
		CCSPlayerPawn* pPlayer = pPlayerController->m_hPlayerPawn().Get();
		if(pPlayer)
		{
			Msg("DEBUG: %f | %f\n", gpGlobals->curtime, fTime);	
			pPlayer->m_flHealthShotBoostExpirationTime().m_Value = gpGlobals->curtime + fTime;
			g_pUtils->SetStateChanged(pPlayer, "CCSPlayerPawn", "m_flHealthShotBoostExpirationTime");
		}
	}
}

void VIP_KS::AllPluginsLoaded()
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
	g_pVIPCore->VIP_RegisterFeature("killscreen", VIP_BOOL, TOGGLABLE);
	g_pUtils->HookEvent(g_PLID, "player_death", OnPlayerDeath);
	g_pUtils->StartupServer(g_PLID, OnStartupServer);
}

const char *VIP_KS::GetLicense()
{
	return "Public";
}

const char *VIP_KS::GetVersion()
{
	return "1.0";
}

const char *VIP_KS::GetDate()
{
	return __DATE__;
}

const char *VIP_KS::GetLogTag()
{
	return "[VIP-KILLSCREEN]";
}

const char *VIP_KS::GetAuthor()
{
	return "Pisex";
}

const char *VIP_KS::GetDescription()
{
	return "";
}

const char *VIP_KS::GetName()
{
	return "[VIP] Kill Screen";
}

const char *VIP_KS::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
