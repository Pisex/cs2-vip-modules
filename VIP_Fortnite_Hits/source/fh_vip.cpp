#include <stdio.h>
#include "fh_vip.h"
#include <fstream>
#include "entitykeyvalues.h"
#include "schemasystem/schemasystem.h"

fh_vip g_fh_vip;
PLUGIN_EXPOSE(fh_vip, g_fh_vip);

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

IUtilsApi* g_pUtils;
IPlayersApi* g_pPlayers;
IFortniteHitsApi* g_pFortniteHits;
IVIPApi* g_pVIPCore;

CGameEntitySystem* GameEntitySystem()
{
	return g_pUtils->GetCGameEntitySystem();
}

void StartupServer()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pUtils->GetCEntitySystem();
}

bool fh_vip::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetServerFactory, g_pSource2GameEntities, ISource2GameEntities, SOURCE2GAMEENTITIES_INTERFACE_VERSION);

	g_SMAPI->AddListener( this, this );
	return true;
}

bool fh_vip::Unload(char *error, size_t maxlen)
{
	ConVar_Unregister();
	return true;
}

void OnClientAuthorized(int iSlot, bool bIsVIP)
{
	if(bIsVIP && g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "fortnite_hits")) {
		g_pFortniteHits->GiveClientAccess(iSlot);
	}
}

bool OnToggle(int iSlot, const char* szFeature, VIP_ToggleState eOldStatus, VIP_ToggleState& eNewStatus)
{
	if (eNewStatus == ENABLED)
		g_pFortniteHits->GiveClientAccess(iSlot);
	else
		g_pFortniteHits->TakeClientAccess(iSlot);
	return false;
}

void fh_vip::AllPluginsLoaded()
{
	char error[64];
	int ret;
	g_pUtils = (IUtilsApi *)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_SMAPI->Format(error, sizeof(error), "Missing Utils system plugin");
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pPlayers = (IPlayersApi *)g_SMAPI->MetaFactory(PLAYERS_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_SMAPI->Format(error, sizeof(error), "Missing Players system plugin");
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pFortniteHits = (IFortniteHitsApi *)g_SMAPI->MetaFactory(FH_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_pUtils->ErrorLog("[%s] Missing Fortnite Hits system plugin", GetLogTag());
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pVIPCore = (IVIPApi *)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_pUtils->ErrorLog("[%s] Missing VIP system plugin", GetLogTag());
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pVIPCore->VIP_OnClientLoaded(OnClientAuthorized);
	g_pVIPCore->VIP_RegisterFeature("fortnite_hits", VIP_BOOL, TOGGLABLE, nullptr, OnToggle);
	g_pUtils->StartupServer(g_PLID, StartupServer);
}

///////////////////////////////////////
const char* fh_vip::GetLicense()
{
	return "GPL";
}

const char* fh_vip::GetVersion()
{
	return "1.0.1";
}

const char* fh_vip::GetDate()
{
	return __DATE__;
}

const char *fh_vip::GetLogTag()
{
	return "fh_vip";
}

const char* fh_vip::GetAuthor()
{
	return "Pisex";
}

const char* fh_vip::GetDescription()
{
	return "[VIP] Fortnite Hits";
}

const char* fh_vip::GetName()
{
	return "[VIP] Fortnite Hits";
}

const char* fh_vip::GetURL()
{
	return "https://discord.gg/g798xERK5Y";
}
