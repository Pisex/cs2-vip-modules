#include <stdio.h>
#include "vip_vampirism.h"

VIPvampirism g_VIPvampirism;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

bool g_bvampirism[64];
PLUGIN_EXPOSE(VIPvampirism, g_VIPvampirism);
bool VIPvampirism::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool VIPvampirism::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	return true;
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pVIPCore->VIP_GetEntitySystem();
};

void OnClientLoaded(int iSlot, bool bIsVIP)
{
	if(bIsVIP)
		g_bvampirism[iSlot] = g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "Vampirism");
	else
		g_bvampirism[iSlot] = false;
}

void OnClientDisconnect(int iSlot, bool bIsVIP)
{
	g_bvampirism[iSlot] = false;
}

bool OnTogglable(int iSlot, const char* szFeature, VIP_ToggleState eOldStatus, VIP_ToggleState& eNewStatus)
{
	g_bvampirism[iSlot] = (eNewStatus == ENABLED);
	return false;
}

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pVIPCore->VIP_OnClientLoaded(OnClientLoaded);
	g_pVIPCore->VIP_OnClientDisconnect(OnClientDisconnect);
}

void OnPlayerHurt(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	int iAttacker = pEvent->GetPlayerSlot("attacker").Get();
	if (iAttacker == pEvent->GetInt("userid")) return;
	CCSPlayerController* pAttacker = (CCSPlayerController*)g_pEntitySystem->GetBaseEntity((CEntityIndex)(iAttacker + 1));
	if(!pAttacker) return;
	CCSPlayerPawnBase* pPlayerPawn = pAttacker->m_hPlayerPawn();
	if (!pPlayerPawn || pPlayerPawn->m_lifeState() != LIFE_ALIVE)
		return;
	int iVamp = g_pVIPCore->VIP_GetClientFeatureInt(iAttacker, "Vampirism");
	if (g_pVIPCore->VIP_IsClientVIP(iAttacker) && iVamp > 0)
	{
		int iHealth = pPlayerPawn->m_iHealth() + round(pEvent->GetInt("dmg_health") * iVamp/100);
		if (iHealth > pPlayerPawn->m_iMaxHealth())
			iHealth = pPlayerPawn->m_iMaxHealth();

		pPlayerPawn->m_iHealth() = iHealth;
		g_pUtils->SetStateChanged(pPlayerPawn, "CBaseEntity", "m_iHealth");
	}
}

void VIPvampirism::AllPluginsLoaded()
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

	g_pUtils = (IUtilsApi *)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		V_strncpy(error, "Missing Utils system plugin", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}

	g_pUtils->HookEvent(g_PLID, "player_hurt", OnPlayerHurt);

	g_pVIPCore->VIP_OnVIPLoaded(VIP_OnVIPLoaded);
	g_pVIPCore->VIP_RegisterFeature("Vampirism", VIP_BOOL, TOGGLABLE, nullptr, OnTogglable);
}

const char *VIPvampirism::GetLicense()
{
	return "Public";
}

const char *VIPvampirism::GetVersion()
{
	return "1.0";
}

const char *VIPvampirism::GetDate()
{
	return __DATE__;
}

const char *VIPvampirism::GetLogTag()
{
	return "[VIP-Vampirism]";
}

const char *VIPvampirism::GetAuthor()
{
	return "Pisex";
}

const char *VIPvampirism::GetDescription()
{
	return "";
}

const char *VIPvampirism::GetName()
{
	return "[VIP] Vampirism";
}

const char *VIPvampirism::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
