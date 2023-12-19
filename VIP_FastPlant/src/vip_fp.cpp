#include <stdio.h>
#include "vip_fp.h"

vip_fp g_vip_fp;

IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
INetworkGameServer* g_pNetworkGameServer = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;
CGlobalVars* gpGlobals = nullptr;

PLUGIN_EXPOSE(vip_fp, g_vip_fp);
SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);
bool vip_fp::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pNetworkServerService, INetworkServerService, NETWORKSERVERSERVICE_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	
	SH_ADD_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_fp::GameFrame), true);
	return true;
}

bool vip_fp::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_fp::GameFrame), true);
	delete g_pVIPCore;
	return true;
}

void VIP_OnFireEvent(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	if(!strcmp(szName, "bomb_beginplant"))
	{
		auto iUserID = pEvent->GetInt("userid");
		bool bValue = g_pVIPCore->VIP_GetClientFeatureBool(iUserID, "fp");
		if(g_pVIPCore->VIP_IsClientVIP(iUserID) && bValue)
		{
			CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(iUserID + 1));
			if(!pPlayerController) return;
			CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
			if (!pPlayerPawn || pPlayerPawn->m_lifeState() != LIFE_ALIVE) return;

			CPlayer_WeaponServices* pWeaponServices = pPlayerPawn->m_pWeaponServices();
			CEntityInstance* hActiveWeapon = (CEntityInstance*)pWeaponServices->m_hActiveWeapon().Get();
			if(hActiveWeapon && strstr(hActiveWeapon->GetClassname(), "c4"))
			{
				CC4* cC4 = (CC4*)hActiveWeapon;
				cC4->m_fArmedTime().m_Value = gpGlobals->curtime;
			}
		}
	}
}

void VIP_OnVIPLoaded()
{
	g_pNetworkGameServer = g_pNetworkServerService->GetIGameServer();
	gpGlobals = g_pNetworkGameServer->GetGlobals();
	g_pGameEntitySystem = g_pVIPCore->VIP_GetEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pVIPCore->VIP_OnFireEvent(VIP_OnFireEvent);
}

void vip_fp::NextFrame(std::function<void()> fn)
{
	m_nextFrame.push_back(fn);
}

void vip_fp::GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	while (!m_nextFrame.empty())
	{
		m_nextFrame.front()();
		m_nextFrame.pop_front();
	}
}

void vip_fp::AllPluginsLoaded()
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
