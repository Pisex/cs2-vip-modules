#include <stdio.h>
#include "vip_bhop.h"
#include "schemasystem/schemasystem.h"

vip_bhop g_vip_bhop;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(vip_bhop, g_vip_bhop);
bool vip_bhop::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_bhop::Unload(char *error, size_t maxlen)
{
	ConVar_Unregister();
	delete g_pUtils;
	delete g_pVIPCore;
	return true;
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pUtils->GetCGameEntitySystem();
};

void OnStartupServer()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
}

void vip_bhop::AllPluginsLoaded()
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
	g_pUtils->StartupServer(g_PLID, OnStartupServer);
	g_pVIPCore->VIP_RegisterFeature("bhop", VIP_BOOL, TOGGLABLE);
	g_pUtils->CreateTimer(0.0f, [](){
		if(g_pVIPCore->VIP_IsVIPLoaded())
		{
			for (int i = 0; i < 64; i++)
			{
				CCSPlayerController* pPlayerController =  CCSPlayerController::FromSlot(i);
				if(!pPlayerController) continue;
				CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
				if (!pPlayerPawn || !pPlayerPawn->IsAlive()) continue;
				if(!pPlayerPawn->m_pMovementServices()) continue;
				if(!g_pVIPCore->VIP_GetClientFeatureBool(i, "bhop")) continue;
				if(pPlayerPawn->m_pMovementServices()->m_nButtons().m_pButtonStates()[0] & (1 << 1) && pPlayerPawn->m_fFlags() & FL_ONGROUND && pPlayerPawn->m_MoveType() != MOVETYPE_LADDER)
				{
					pPlayerPawn->m_vecAbsVelocity().z = 300;
				}
			}
		}
		return 0.0f;
	});
}

const char *vip_bhop::GetLicense()
{
	return "Public";
}

const char *vip_bhop::GetVersion()
{
	return "1.0";
}

const char *vip_bhop::GetDate()
{
	return __DATE__;
}

const char *vip_bhop::GetLogTag()
{
	return "[VIP-BHOP]";
}

const char *vip_bhop::GetAuthor()
{
	return "Pisex";
}

const char *vip_bhop::GetDescription()
{
	return "";
}

const char *vip_bhop::GetName()
{
	return "[VIP] BHOP";
}

const char *vip_bhop::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
