#include <stdio.h>
#include "vip_jumps.h"
#include "schemasystem/schemasystem.h"

vip_jumps g_vip_jumps;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

struct User
{
    int LastButtons;
    int LastFlags;
    int JumpsCount;
    int NumberOfJumps;
};

CGameEntitySystem* GameEntitySystem()
{
    return g_pUtils->GetCGameEntitySystem();
};

User UserSettings[64];

PLUGIN_EXPOSE(vip_jumps, g_vip_jumps);
bool vip_jumps::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_jumps::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	delete g_pUtils;
	return true;
}

void OnStartupServer()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
}

void OnPlayerSpawn(int iSlot, int iTeam, bool bIsVIP)
{
	if(bIsVIP)
	{
		UserSettings[iSlot].NumberOfJumps = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "jumps");
	}
}

void vip_jumps::AllPluginsLoaded()
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
	g_pVIPCore->VIP_OnPlayerSpawn(OnPlayerSpawn);
	g_pVIPCore->VIP_RegisterFeature("jumps", VIP_INT, TOGGLABLE);
	g_pUtils->CreateTimer(0.0f, [](){
		if(g_pVIPCore->VIP_IsVIPLoaded())
		{
			for (int i = 0; i < 64; i++)
			{
				CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(i);
				if(!pPlayerController) continue;
				CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
				if (!pPlayerPawn || !pPlayerPawn->IsAlive()) continue;
				if (!pPlayerPawn->m_pMovementServices()) continue;
				if(!g_pVIPCore->VIP_GetClientFeatureBool(i, "jumps")) continue;

				int flags = pPlayerPawn->m_fFlags();
				int buttons = pPlayerPawn->m_pMovementServices()->m_nButtons().m_pButtonStates()[0];
				
				if ((flags & FL_ONGROUND) != 0)
					UserSettings[i].JumpsCount = 0;
				else if ((UserSettings[i].LastButtons & (1 << 1)) == 0 && (buttons & (1 << 1)) != 0 && UserSettings[i].JumpsCount < UserSettings[i].NumberOfJumps)
				{
					UserSettings[i].JumpsCount ++;
					pPlayerPawn->m_vecAbsVelocity().z = 300;
				}

				UserSettings[i].LastFlags = flags;
				UserSettings[i].LastButtons = buttons;
			}
		}
		return 0.0f;
	});
}

const char *vip_jumps::GetLicense()
{
	return "Public";
}

const char *vip_jumps::GetVersion()
{
	return "1.0";
}

const char *vip_jumps::GetDate()
{
	return __DATE__;
}

const char *vip_jumps::GetLogTag()
{
	return "[VIP-JUMPS]";
}

const char *vip_jumps::GetAuthor()
{
	return "Pisex";
}

const char *vip_jumps::GetDescription()
{
	return "";
}

const char *vip_jumps::GetName()
{
	return "[VIP] JUMPS";
}

const char *vip_jumps::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
