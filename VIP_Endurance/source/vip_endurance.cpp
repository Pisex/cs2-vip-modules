#include <stdio.h>
#include "vip_endurance.h"

VIPEndurance g_VIPEndurance;

IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

bool g_bEndurance[64];
PLUGIN_EXPOSE(VIPEndurance, g_VIPEndurance);
SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);
bool VIPEndurance::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	SH_ADD_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &VIPEndurance::GameFrame), true);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool VIPEndurance::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &VIPEndurance::GameFrame), true);
	delete g_pVIPCore;
	return true;
}

void VIPEndurance::GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	if(g_pVIPCore->VIP_IsVIPLoaded())
	{
		for (int i = 0; i < 64; i++)
		{
			CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(i + 1));
			if(!pPlayerController) continue;
			CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
			if (!pPlayerPawn || pPlayerPawn->m_lifeState() != LIFE_ALIVE)
				continue;
			if(g_bEndurance[i] && pPlayerPawn->m_flVelocityModifier() < 1.0)
			{
				pPlayerPawn->m_flVelocityModifier() = 1.0;
			}
		}
	}
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pVIPCore->VIP_GetEntitySystem();
};

void OnClientLoaded(int iSlot, bool bIsVIP)
{
	if(bIsVIP)
		g_bEndurance[iSlot] = g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "Endurance");
	else
		g_bEndurance[iSlot] = false;
}

void OnClientDisconnect(int iSlot, bool bIsVIP)
{
	g_bEndurance[iSlot] = false;
}

bool OnTogglable(int iSlot, const char* szFeature, VIP_ToggleState eOldStatus, VIP_ToggleState& eNewStatus)
{
	g_bEndurance[iSlot] = (eNewStatus == ENABLED);
	return false;
}

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pVIPCore->VIP_OnClientLoaded(OnClientLoaded);
	g_pVIPCore->VIP_OnClientDisconnect(OnClientDisconnect);
}

void VIPEndurance::AllPluginsLoaded()
{
	char error[64] = { 0 };
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
	g_pVIPCore->VIP_RegisterFeature("Endurance", VIP_BOOL, TOGGLABLE, nullptr, OnTogglable);
}

const char *VIPEndurance::GetLicense()
{
	return "Public";
}

const char *VIPEndurance::GetVersion()
{
	return "1.0";
}

const char *VIPEndurance::GetDate()
{
	return __DATE__;
}

const char *VIPEndurance::GetLogTag()
{
	return "[VIP-Endurance]";
}

const char *VIPEndurance::GetAuthor()
{
	return "Pisex";
}

const char *VIPEndurance::GetDescription()
{
	return "";
}

const char *VIPEndurance::GetName()
{
	return "[VIP] Endurance";
}

const char *VIPEndurance::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
