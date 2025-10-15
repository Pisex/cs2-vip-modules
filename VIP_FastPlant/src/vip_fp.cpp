#include <stdio.h>
#include "vip_fp.h"
#include "schemasystem/schemasystem.h"

vip_fp g_vip_fp;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;
CGlobalVars* gpGlobals = nullptr;

PLUGIN_EXPOSE(vip_fp, g_vip_fp);
SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);
bool vip_fp::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pNetworkServerService, INetworkServerService, NETWORKSERVERSERVICE_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_fp::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	delete g_pUtils;
	return true;
}

void OnBeginPlant(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	int iSlot = pEvent->GetInt("userid");
	bool bValue = g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "fp");
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && bValue)
	{
		CCSPlayerController* pPlayerController =  CCSPlayerController::FromSlot(iSlot);
		if(!pPlayerController) return;
		CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
		if (!pPlayerPawn || !pPlayerPawn->IsAlive()) return;
		CPlayer_WeaponServices* pWeaponServices = pPlayerPawn->m_pWeaponServices();
		if(!pWeaponServices) return;
		CEntityInstance* hActiveWeapon = (CEntityInstance*)pWeaponServices->m_hActiveWeapon();
		if(hActiveWeapon && strstr(hActiveWeapon->GetClassname(), "c4"))
		{
			CC4* cC4 = (CC4*)hActiveWeapon;
			cC4->m_fArmedTime().SetTime(gpGlobals->curtime);
		}
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

void vip_fp::AllPluginsLoaded()
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
	g_pVIPCore->VIP_RegisterFeature("fp", VIP_BOOL, TOGGLABLE);
	g_pUtils->StartupServer(g_PLID, OnStartupServer);
	g_pUtils->HookEvent(g_PLID, "bomb_beginplant",OnBeginPlant);
}

const char *vip_fp::GetLicense()
{
	return "Public";
}

const char *vip_fp::GetVersion()
{
	return "1.0";
}

const char *vip_fp::GetDate()
{
	return __DATE__;
}

const char *vip_fp::GetLogTag()
{
	return "[VIP-FP]";
}

const char *vip_fp::GetAuthor()
{
	return "Pisex";
}

const char *vip_fp::GetDescription()
{
	return "";
}

const char *vip_fp::GetName()
{
	return "[VIP] Fast Plant";
}

const char *vip_fp::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
