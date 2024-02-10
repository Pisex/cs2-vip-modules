#include <stdio.h>
#include "vip_jumps.h"

vip_jumps g_vip_jumps;

IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
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
    return g_pVIPCore->VIP_GetEntitySystem();
};

User UserSettings[64];

SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);
PLUGIN_EXPOSE(vip_jumps, g_vip_jumps);
bool vip_jumps::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	SH_ADD_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_jumps::GameFrame), true);
	return true;
}

void vip_jumps::GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	if(g_pVIPCore->VIP_IsVIPLoaded())
	{
		for (int i = 0; i < 65; i++)
		{
			CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(i + 1));
			if(!pPlayerController) continue;
			CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
			if (!pPlayerPawn || pPlayerPawn->m_lifeState() != LIFE_ALIVE)
				continue;
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
}

bool vip_jumps::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_jumps::GameFrame), true);
	delete g_pVIPCore;
	return true;
}

void VIP_OnVIPLoaded()
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
	g_pVIPCore->VIP_OnPlayerSpawn(OnPlayerSpawn);
	g_pVIPCore->VIP_RegisterFeature("jumps", VIP_INT, TOGGLABLE);
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
