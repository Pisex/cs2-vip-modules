#include <stdio.h>
#include "vip_bf.h"
#include <sstream>
#include "schemasystem/schemasystem.h"

vip_bf g_vip_bf;

IUtilsApi* g_pUtils;
IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

int iRoundMin;

PLUGIN_EXPOSE(vip_bf, g_vip_bf);
bool vip_bf::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);

	{
		KeyValues* hKv = new KeyValues("VIP");
		const char *pszPath = "addons/configs/vip/vip_bf.ini";

		if (!hKv->LoadFromFile(g_pFullFileSystem, pszPath))
		{
			Warning("Failed to load %s\n", pszPath);
			return false;
		}

		iRoundMin = hKv->GetInt("round_min");
		delete hKv;
	}

	g_SMAPI->AddListener( this, this );
	return true;
}

void VIP_OnPlayerSpawn(int iSlot, int iTeam, bool bIsVIP)
{
	if(bIsVIP)
	{
		if((iRoundMin == 1 && !g_pVIPCore->VIP_PistolRound()) || (iRoundMin <= g_pVIPCore->VIP_GetTotalRounds() && !g_pVIPCore->VIP_PistolRound()) || (iRoundMin == 0))
		{
			CCSPlayerController* pPlayerController =  CCSPlayerController::FromSlot(iSlot);
			if(!pPlayerController) return;
			CCSPlayerPawn* pPlayerPawn = pPlayerController->m_hPlayerPawn();
			if (!pPlayerPawn || !pPlayerPawn->IsAlive()) return;
			const char* sHealth = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "health");
			if(strlen(sHealth) > 0)
			{
				if(sHealth[0] == '+')
				{
					std::string str(sHealth);
					str.erase(0,2);
					pPlayerPawn->m_iMaxHealth() += atoi(str.c_str());
					pPlayerPawn->m_iHealth() += atoi(str.c_str());
				}
				else
				{
					pPlayerPawn->m_iMaxHealth() = atoi(sHealth);
					pPlayerPawn->m_iHealth() = atoi(sHealth);
				}
			}

			const char* sArmor = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "armor");
			if(strlen(sArmor) > 0)
			{
				if(sArmor[0] == '+')
				{
					std::string str(sArmor);
					str.erase(0,2);
					pPlayerPawn->m_ArmorValue() += atoi(str.c_str());
				}
				else
					pPlayerPawn->m_ArmorValue() = atoi(sArmor);
			}

			CCSPlayerController_InGameMoneyServices* pMoneyServices = pPlayerController->m_pInGameMoneyServices();
			if(!pMoneyServices) return;
			const char* sMoney = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "money");
			if(strlen(sMoney) > 0)
			{
				if(sMoney[0] == '+')
				{
					std::string str(sMoney);
					str.erase(0,2);
					pMoneyServices->m_iAccount() += atoi(str.c_str());
				}
				else
					pMoneyServices->m_iAccount() = atoi(sMoney);
			}

			CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
			if(!pItemServices) return;
			int bHelmet = g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "helmet");
			if (bHelmet)
			{
				pItemServices->m_bHasHelmet() = true;
			}

			int bDefuser = g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "defuser");
			if (bDefuser && pPlayerPawn->m_iTeamNum() == 3)
			{
				pItemServices->m_bHasDefuser() = true;
			}
		}
	}
}

bool vip_bf::Unload(char *error, size_t maxlen)
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

void vip_bf::AllPluginsLoaded()
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
	g_pVIPCore->VIP_RegisterFeature("health", 	VIP_STRING, TOGGLABLE);
	g_pVIPCore->VIP_RegisterFeature("defuser", 	VIP_STRING, TOGGLABLE);
	g_pVIPCore->VIP_RegisterFeature("money", 	VIP_STRING, TOGGLABLE);
	g_pVIPCore->VIP_RegisterFeature("helmet", 	VIP_BOOL, TOGGLABLE);
	g_pVIPCore->VIP_RegisterFeature("defuser", 	VIP_BOOL, TOGGLABLE);
}

const char *vip_bf::GetLicense()
{
	return "Public";
}

const char *vip_bf::GetVersion()
{
	return "1.1";
}

const char *vip_bf::GetDate()
{
	return __DATE__;
}

const char *vip_bf::GetLogTag()
{
	return "[VIP-BF]";
}

const char *vip_bf::GetAuthor()
{
	return "Pisex";
}

const char *vip_bf::GetDescription()
{
	return "";
}

const char *vip_bf::GetName()
{
	return "[VIP] Base Functions";
}

const char *vip_bf::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
