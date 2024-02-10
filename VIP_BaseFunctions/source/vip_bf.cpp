#include <stdio.h>
#include "vip_bf.h"

vip_bf g_vip_bf;

IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

int iRoundMin;

PLUGIN_EXPOSE(vip_bf, g_vip_bf);
bool vip_bf::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
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
			CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(iSlot + 1));
			if(!pPlayerController) return;
			CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
			if (!pPlayerPawn || pPlayerPawn->m_lifeState() != LIFE_ALIVE)
				return;
			CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());

			const char* sHealth = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "health");
			if(strlen(sHealth) > 0)
			{
				if(sHealth[0] == '+')
				{
					std::string str(sHealth);
					str.erase(0,2);
					pPlayerPawn->m_iMaxHealth() += std::stoi(str);
					pPlayerPawn->m_iHealth() += std::stoi(str);
				}
				else
				{
					pPlayerPawn->m_iMaxHealth() = std::stoi(sHealth);
					pPlayerPawn->m_iHealth() = std::stoi(sHealth);
				}
			}

			const char* sArmor = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "armor");
			if(strlen(sArmor) > 0)
			{
				if(sArmor[0] == '+')
				{
					std::string str(sArmor);
					str.erase(0,2);
					pPlayerPawn->m_ArmorValue() += std::stoi(str);
				}
				else
					pPlayerPawn->m_ArmorValue() = std::stoi(sArmor);
			}

			CCSPlayerController_InGameMoneyServices* pMoneyServices = pPlayerController->m_pInGameMoneyServices();

			const char* sMoney = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "money");
			if(strlen(sMoney) > 0)
			{
				if(sMoney[0] == '+')
				{
					std::string str(sMoney);
					str.erase(0,2);
					pMoneyServices->m_iAccount() += std::stoi(str);
				}
				else
					pMoneyServices->m_iAccount() = std::stoi(sMoney);
			}

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
	return true;
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pVIPCore->VIP_GetEntitySystem();
};

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pVIPCore->VIP_OnPlayerSpawn(VIP_OnPlayerSpawn);
}

void vip_bf::AllPluginsLoaded()
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
	g_pVIPCore->VIP_RegisterFeature("health", 	VIP_STRING, TOGGLABLE);
	g_pVIPCore->VIP_RegisterFeature("armor", 	VIP_STRING, TOGGLABLE);
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
	return "1.0";
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
