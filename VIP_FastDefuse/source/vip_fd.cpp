#include <stdio.h>
#include "vip_fd.h"
#include "schemasystem/schemasystem.h"

vip_fd g_vip_fd;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;
CGlobalVars* gpGlobals = nullptr;

PLUGIN_EXPOSE(vip_fd, g_vip_fd);
SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);
bool vip_fd::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pNetworkServerService, INetworkServerService, NETWORKSERVERSERVICE_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	
	return true;
}

bool vip_fd::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	delete g_pUtils;
	return true;
}

void OnBeginDefuse(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	auto iSlot = pEvent->GetInt("userid");
	int iValue = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "fd");
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && iValue)
	{
		g_pUtils->NextFrame([iSlot, iValue](){
			CCSPlayerController* pPlayerController =  CCSPlayerController::FromSlot(iSlot);
			if(!pPlayerController) return;
			CCSPlayerPawn* pPlayerPawn = pPlayerController->GetPlayerPawn();
			if (!pPlayerPawn || !pPlayerPawn->IsAlive()) return;
			CPlantedC4* pBomb = (CPlantedC4*)UTIL_FindEntityByClassname("planted_c4");
			if(!pBomb) return;
			float fCountDown;
			if(pBomb->m_flDefuseCountDown().GetTime() < gpGlobals->curtime)
				fCountDown = 10.0;
			else
				fCountDown = pBomb->m_flDefuseCountDown().GetTime() - gpGlobals->curtime;
			fCountDown -= fCountDown/100.0*float(iValue);
			pBomb->m_flDefuseCountDown().SetTime(fCountDown + gpGlobals->curtime);
			pPlayerPawn->m_iProgressBarDuration() = ceil(fCountDown);
		});
	}
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pUtils->GetCGameEntitySystem();
};

void OnStartupServer()
{
	gpGlobals = g_pUtils->GetCGlobalVars();
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pUtils->GetCEntitySystem();
}

void vip_fd::AllPluginsLoaded()
{
	int ret;
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
	g_pVIPCore = (IVIPApi*)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_pUtils->ErrorLog("[%s] Failed to lookup vip core. Aborting", GetLogTag());
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pVIPCore->VIP_RegisterFeature("fd", VIP_BOOL, TOGGLABLE);
	g_pUtils->StartupServer(g_PLID, OnStartupServer);
	g_pUtils->HookEvent(g_PLID, "bomb_begindefuse", OnBeginDefuse);
}

const char *vip_fd::GetLicense()
{
	return "Public";
}

const char *vip_fd::GetVersion()
{
	return "1.0";
}

const char *vip_fd::GetDate()
{
	return __DATE__;
}

const char *vip_fd::GetLogTag()
{
	return "[VIP-FD]";
}

const char *vip_fd::GetAuthor()
{
	return "Pisex";
}

const char *vip_fd::GetDescription()
{
	return "";
}

const char *vip_fd::GetName()
{
	return "[VIP] Fast Defuse";
}

const char *vip_fd::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
