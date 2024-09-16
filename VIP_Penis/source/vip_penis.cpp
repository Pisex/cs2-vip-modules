#include <stdio.h>
#include "vip_penis.h"
#include "metamod_oslink.h"
#include "entitykeyvalues.h"
#include "schemasystem/schemasystem.h"

vip_penis g_vip_penis;
PLUGIN_EXPOSE(vip_penis, g_vip_penis);
IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;
CGlobalVars *gpGlobals = nullptr;

IUtilsApi* g_pUtils;
IVIPApi* g_pVIPCore;

CHandle<CBaseModelEntity> g_hPenis[64];

bool g_bPenis[64];

CGameEntitySystem* GameEntitySystem()
{
	return g_pUtils->GetCGameEntitySystem();
}

void StartupServer()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pUtils->GetCEntitySystem();
	gpGlobals = g_pUtils->GetCGlobalVars();
}

bool vip_penis::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);

	g_SMAPI->AddListener( this, this );

	return true;
}

bool vip_penis::Unload(char *error, size_t maxlen)
{
	ConVar_Unregister();
	
	return true;
}

void TryRemoveOldPenis(int iSlot)
{
	if(g_hPenis[iSlot])
	{
		g_pUtils->RemoveEntity(g_hPenis[iSlot]);
		g_hPenis[iSlot] = nullptr;
	}
}

void TryCreatePenis(int iSlot)
{
	TryRemoveOldPenis(iSlot);
	if(g_bPenis[iSlot])
	{
		CCSPlayerController* player =  CCSPlayerController::FromSlot(iSlot);
		if (!player || !player->m_hPlayerPawn()) return;
		CCSPlayerPawn* pPlayer = player->m_hPlayerPawn();
		if(!pPlayer || !pPlayer->IsAlive()) return;

		CBaseModelEntity* pPenis = (CBaseModelEntity*)g_pUtils->CreateEntityByName("prop_dynamic", -1);
		CEntityKeyValues* pKeyValues = new CEntityKeyValues();
		pKeyValues->SetVector("origin", pPlayer->GetAbsOrigin());
		pKeyValues->SetString("model", g_pVIPCore->VIP_GetClientFeatureString(iSlot, "penis_model"));
		g_pUtils->DispatchSpawn(pPenis, pKeyValues);
		g_pUtils->AcceptEntityInput(pPenis, "FollowEntity", "!activator", pPlayer, pPenis);
		g_hPenis[iSlot] = CHandle<CBaseModelEntity>(pPenis);
	}
}

bool OnToggle(int iSlot, const char* szFeature, VIP_ToggleState eOldStatus, VIP_ToggleState& eNewStatus)
{
	g_bPenis[iSlot] = (eNewStatus == ENABLED);
	if(g_bPenis[iSlot]) TryCreatePenis(iSlot);
	else TryRemoveOldPenis(iSlot);
	return false;
}

void OnPlayerSpawn(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	int iSlot = pEvent->GetInt("userid");
	TryCreatePenis(iSlot);
}

void OnClientLoad(int iSlot, bool bIsVIP)
{
	if(g_hPenis[iSlot])
	{
		g_pUtils->RemoveEntity(g_hPenis[iSlot]);
		g_hPenis[iSlot] = nullptr;
	}

	if(bIsVIP)
		g_bPenis[iSlot] = g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "penis");
	else 
		g_bPenis[iSlot] = false;
}

void OnClientRemoveVIP(int iSlot, int iReason)
{
	TryRemoveOldPenis(iSlot);
	g_bPenis[iSlot] = false;
}

void vip_penis::AllPluginsLoaded()
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
	g_pVIPCore = (IVIPApi *)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_SMAPI->Format(error, sizeof(error), "Missing VIP system plugin");
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pUtils->StartupServer(g_PLID, StartupServer);
	g_pVIPCore->VIP_OnClientLoaded(OnClientLoad);
	g_pVIPCore->VIP_OnVIPClientRemoved(OnClientRemoveVIP);
	g_pUtils->HookEvent(g_PLID, "player_spawn", OnPlayerSpawn);
	g_pVIPCore->VIP_RegisterFeature("penis", VIP_INT, TOGGLABLE, nullptr, OnToggle);
	g_pVIPCore->VIP_RegisterFeature("penis_model", VIP_STRING, HIDE);
}

///////////////////////////////////////
const char* vip_penis::GetLicense()
{
	return "GPL";
}

const char* vip_penis::GetVersion()
{
	return "1.0";
}

const char* vip_penis::GetDate()
{
	return __DATE__;
}

const char *vip_penis::GetLogTag()
{
	return "vip_penis";
}

const char* vip_penis::GetAuthor()
{
	return "Pisex";
}

const char* vip_penis::GetDescription()
{
	return "[VIP] Penis";
}

const char* vip_penis::GetName()
{
	return "[VIP] Penis";
}

const char* vip_penis::GetURL()
{
	return "https://discord.gg/g798xERK5Y";
}
