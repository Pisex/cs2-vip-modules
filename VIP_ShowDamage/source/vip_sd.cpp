#include <stdio.h>
#include "vip_sd.h"

vip_sd g_vip_sd;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(vip_sd, g_vip_sd);
bool vip_sd::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_sd::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	g_pUtils->ClearAllHooks(g_PLID);
	delete g_pUtils;
	return true;
}

void OnPlayerHurt(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	int iSlot = pEvent->GetPlayerSlot("attacker").Get();
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "show_damage"))
	{
		g_pVIPCore->VIP_PrintToCenter(pEvent->GetPlayerSlot("attacker").Get(), "-%i", pEvent->GetInt("dmg_health"));
	}
}


CGameEntitySystem* GameEntitySystem()
{
    return g_pVIPCore->VIP_GetEntitySystem();
};

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pUtils->HookEvent(g_PLID, "player_hurt", OnPlayerHurt);
}

void vip_sd::AllPluginsLoaded()
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
	g_pVIPCore->VIP_RegisterFeature("show_damage", VIP_BOOL, TOGGLABLE);
}

const char *vip_sd::GetLicense()
{
	return "Public";
}

const char *vip_sd::GetVersion()
{
	return "1.0";
}

const char *vip_sd::GetDate()
{
	return __DATE__;
}

const char *vip_sd::GetLogTag()
{
	return "[VIP-SD]";
}

const char *vip_sd::GetAuthor()
{
	return "Pisex";
}

const char *vip_sd::GetDescription()
{
	return "";
}

const char *vip_sd::GetName()
{
	return "[VIP] Show Damage";
}

const char *vip_sd::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
