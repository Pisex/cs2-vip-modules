#include <stdio.h>
#include "vip_tag.h"

VIPTag g_VIPTag;

IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(VIPTag, g_VIPTag);
bool VIPTag::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool VIPTag::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	return true;
}

void VIP_OnPlayerSpawn(int iSlot, int iTeam, bool bIsVIP)
{
	if(bIsVIP)
	{
		CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
		if(!pPlayerController) return;
		const char* szClan = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "clantag");
		if(strlen(szClan) > 0)
			pPlayerController->m_szClan() = CUtlSymbolLarge(szClan);
		else
			pPlayerController->m_szClan() = CUtlSymbolLarge("\0");
	}
}

void VIP_OnVIPClientRemoved(int iSlot, int iReason)
{
	CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
	if(!pPlayerController) return;
	pPlayerController->m_szClan() = CUtlSymbolLarge("\0");
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pVIPCore->VIP_GetEntitySystem();
};

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pVIPCore->VIP_OnPlayerSpawn(VIP_OnPlayerSpawn);
	g_pVIPCore->VIP_OnVIPClientRemoved(VIP_OnVIPClientRemoved);
}

void VIPTag::AllPluginsLoaded()
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
	g_pVIPCore->VIP_RegisterFeature("clantag", VIP_STRING, TOGGLABLE);
}

const char *VIPTag::GetLicense()
{
	return "Public";
}

const char *VIPTag::GetVersion()
{
	return "1.0";
}

const char *VIPTag::GetDate()
{
	return __DATE__;
}

const char *VIPTag::GetLogTag()
{
	return "[VIP-TAG]";
}

const char *VIPTag::GetAuthor()
{
	return "Pisex";
}

const char *VIPTag::GetDescription()
{
	return "Puts a vip tag on the player";
}

const char *VIPTag::GetName()
{
	return "[VIP] Tag";
}

const char *VIPTag::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}