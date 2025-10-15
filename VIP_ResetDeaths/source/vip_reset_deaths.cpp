#include <stdio.h>
#include "vip_reset_deaths.h"
#include "schemasystem/schemasystem.h"

VIPResetDeaths g_VIPResetDeaths;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

bool g_bResetDeaths[64];

PLUGIN_EXPOSE(VIPResetDeaths, g_VIPResetDeaths);

bool VIPResetDeaths::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener(this, this);
	return true;
}

bool VIPResetDeaths::Unload(char* error, size_t maxlen)
{
	delete g_pVIPCore;
	delete g_pUtils;
	return true;
}

void OnClientLoaded(int iSlot, bool bIsVIP)
{
	if (bIsVIP)
		g_bResetDeaths[iSlot] = g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "ResetDeaths");
	else
		g_bResetDeaths[iSlot] = false;
}

void OnClientDisconnect(int iSlot, bool bIsVIP)
{
	g_bResetDeaths[iSlot] = false;
}

bool OnTogglable(int iSlot, const char* szFeature, VIP_ToggleState eOldStatus, VIP_ToggleState& eNewStatus)
{
	g_bResetDeaths[iSlot] = (eNewStatus == ENABLED);
	return false;
}

bool VIP_ResetDeathsCommand(int iSlot, const char* szContent)
{
	if (!g_bResetDeaths[iSlot])
	{
		g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("NotAccess"));
		return true; 
	}

	CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
	if (!pPlayerController) return false;

	CCSPlayerController_ActionTrackingServices* m_ATS = pPlayerController->m_pActionTrackingServices();

	if (m_ATS->m_matchStats().m_iDeaths() == 0)
	{
		g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("deaths_already_zero"));
		return true;
	}

	m_ATS->m_matchStats().m_iDeaths() = 0;

	g_pUtils->SetStateChanged(pPlayerController, "CCSPlayerController", "m_pActionTrackingServices");
	g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("deaths_reset"));

	return true;
}

bool ResetKillsCommand(int iSlot, const char* szContent)
{
	CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
	if (!pPlayerController) return false;

	CCSPlayerController_ActionTrackingServices* m_ATS = pPlayerController->m_pActionTrackingServices();

	if (m_ATS->m_matchStats().m_iKills() == 0)
	{
		g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("kills_already_zero")); 
		return true;
	}

	m_ATS->m_matchStats().m_iKills() = 0;

	g_pUtils->SetStateChanged(pPlayerController, "CCSPlayerController", "m_pActionTrackingServices");

	g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("kills_reset")); 

	return true;
}

CGameEntitySystem* GameEntitySystem()
{
	return g_pUtils->GetCGameEntitySystem();
}

void OnStartupServer()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
}

void VIPResetDeaths::AllPluginsLoaded()
{
	int ret;
	g_pUtils = (IUtilsApi*)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		char error[64];
		V_strncpy(error, "Failed to lookup utils api. Aborting", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload " + std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pVIPCore = (IVIPApi*)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_pUtils->ErrorLog("[%s] Failed to lookup vip core. Aborting", GetLogTag());
		std::string sBuffer = "meta unload " + std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}

	g_pUtils->StartupServer(g_PLID, OnStartupServer);
	g_pVIPCore->VIP_OnClientLoaded(OnClientLoaded);
	g_pVIPCore->VIP_OnClientDisconnect(OnClientDisconnect);

	g_pVIPCore->VIP_RegisterFeature("ResetDeaths", VIP_BOOL, SELECTABLE, VIP_ResetDeathsCommand, nullptr);

	g_pUtils->RegCommand(g_PLID, { "sm_rd", "mm_rd" }, { "!rd", "rd" }, VIP_ResetDeathsCommand);
	g_pUtils->RegCommand(g_PLID, { "sm_rs", "mm_rs" }, { "!rs", "rs" }, ResetKillsCommand);      
}

const char* VIPResetDeaths::GetLicense()
{
	return "Public";
}

const char* VIPResetDeaths::GetVersion()
{
	return "1.1";
}

const char* VIPResetDeaths::GetDate()
{
	return __DATE__;
}

const char* VIPResetDeaths::GetLogTag()
{
	return "[VIP-Reset-Deaths]";
}

const char* VIPResetDeaths::GetAuthor()
{
	return "ABKAM";
}

const char* VIPResetDeaths::GetDescription()
{
	return "A plugin to reset death count for VIP players and kills for all players.";
}

const char* VIPResetDeaths::GetName()
{
	return "[VIP] Reset Deaths";
}

const char* VIPResetDeaths::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
