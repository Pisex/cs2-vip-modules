#include <stdio.h>
#include "vip_healthshot.h"
#include "schemasystem/schemasystem.h"

vip_healthshot g_vip_healthshot;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

int iRoundMin;

PLUGIN_EXPOSE(vip_healthshot, g_vip_healthshot);
bool vip_healthshot::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );

	{
		KeyValues* hKv = new KeyValues("VIP");
		const char *pszPath = "addons/configs/vip/vip_healthshot.ini";

		if (!hKv->LoadFromFile(g_pFullFileSystem, pszPath))
		{
			Warning("Failed to load %s\n", pszPath);
			return false;
		}

		iRoundMin = hKv->GetInt("round_min");
		delete hKv;
	}

	return true;
}

void VIP_OnPlayerSpawn(int iSlot, int iTeam, bool bIsVIP)
{
	if(bIsVIP)
	{
		if((iRoundMin == 1 && !g_pVIPCore->VIP_PistolRound()) || (iRoundMin <= g_pVIPCore->VIP_GetTotalRounds() && !g_pVIPCore->VIP_PistolRound()) || (iRoundMin == 0))
		{
			CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
			if(!pPlayerController) return;
			CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
			if (!pPlayerPawn || !pPlayerPawn->IsAlive()) return;
			CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
			if(!pItemServices) return;
			CPlayer_WeaponServices* pWeaponServices = pPlayerPawn->m_pWeaponServices();
			if(!pWeaponServices) return;
			int iCurrent = pWeaponServices->m_iAmmo()[20];
			int iCount = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "healthshot");
			for(int i = 0; i < iCount - iCurrent; i++){
				pItemServices->GiveNamedItem("weapon_healthshot");
			}
		}
	}
}

bool vip_healthshot::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	delete g_pUtils;
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

void vip_healthshot::AllPluginsLoaded()
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
	g_pVIPCore->VIP_OnPlayerSpawn(VIP_OnPlayerSpawn);
	g_pVIPCore->VIP_RegisterFeature("healthshot", VIP_INT, TOGGLABLE);
}

const char *vip_healthshot::GetLicense()
{
	return "Public";
}

const char *vip_healthshot::GetVersion()
{
	return "1.0";
}

const char *vip_healthshot::GetDate()
{
	return __DATE__;
}

const char *vip_healthshot::GetLogTag()
{
	return "[VIP-HEALTHSHOT]";
}

const char *vip_healthshot::GetAuthor()
{
	return "Pisex";
}

const char *vip_healthshot::GetDescription()
{
	return "";
}

const char *vip_healthshot::GetName()
{
	return "[VIP] Healthshot";
}

const char *vip_healthshot::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
