#include <stdio.h>
#include "vip_items.h"

vip_items g_vip_items;

IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

int iRoundMin;

PLUGIN_EXPOSE(vip_items, g_vip_items);
bool vip_items::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );

	{
		KeyValues* hKv = new KeyValues("VIP");
		const char *pszPath = "addons/configs/vip/vip_items.ini";

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
			if(g_pVIPCore->VIP_GetClientFeatureString(iSlot, "items")[0])
			{
				CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(iSlot + 1));
				if(!pPlayerController) return;
				CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
				if (!pPlayerPawn || pPlayerPawn->m_lifeState() != LIFE_ALIVE)
					return;
				CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
				CUtlVector<char*> m_items;
				CPlayer_WeaponServices* pWeaponServices = pPlayerPawn->m_pWeaponServices();
				std::vector<std::string> hWeapons;
				for (size_t i = 0; i < pWeaponServices->m_hMyWeapons().Count(); i++)
				{
					CEntityInstance* hActiveWeapon = (CEntityInstance*)pWeaponServices->m_hMyWeapons()[i].Get();
					if(!hActiveWeapon) continue;
					hWeapons.push_back(std::string(hActiveWeapon->GetClassname()));
				}

				if (const char* pszItems = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "items"))
				{
					V_SplitString(pszItems, " ", m_items);
				}
				for (int i = 0; i < m_items.Count(); i++)
				{
					if(!strcmp(m_items[i], "weapon_smokegrenade")) {
						if(!pWeaponServices->m_iAmmo()[15]) pItemServices->GiveNamedItem(m_items[i]);
					} else if(!strcmp(m_items[i], "weapon_flashbang")) {
						if(!pWeaponServices->m_iAmmo()[14]) pItemServices->GiveNamedItem(m_items[i]);
					} else if(!strcmp(m_items[i], "weapon_decoy")) {
						if(!pWeaponServices->m_iAmmo()[17]) pItemServices->GiveNamedItem(m_items[i]);
					} else if(!strcmp(m_items[i], "weapon_molotov")) {
						if(!pWeaponServices->m_iAmmo()[16]) pItemServices->GiveNamedItem(m_items[i]);
					} else if(!strcmp(m_items[i], "weapon_incgrenade")) {
						if(!pWeaponServices->m_iAmmo()[16]) pItemServices->GiveNamedItem(m_items[i]);
					} else if(!strcmp(m_items[i],"weapon_hegrenade")) {
						if(!pWeaponServices->m_iAmmo()[13]) pItemServices->GiveNamedItem(m_items[i]);
					} else {
						auto it = std::find(hWeapons.begin(), hWeapons.end(), m_items[i]);
						if (it == hWeapons.end()) {
							pItemServices->GiveNamedItem(m_items[i]);
						}
					}
				}
				m_items.PurgeAndDeleteElements();
				hWeapons.clear();
			}
		}
	}
}

bool vip_items::Unload(char *error, size_t maxlen)
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

void vip_items::AllPluginsLoaded()
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
	g_pVIPCore->VIP_RegisterFeature("items", VIP_STRING, TOGGLABLE);
}

const char *vip_items::GetLicense()
{
	return "Public";
}

const char *vip_items::GetVersion()
{
	return "1.0";
}

const char *vip_items::GetDate()
{
	return __DATE__;
}

const char *vip_items::GetLogTag()
{
	return "[VIP-ITEMS]";
}

const char *vip_items::GetAuthor()
{
	return "Pisex";
}

const char *vip_items::GetDescription()
{
	return "";
}

const char *vip_items::GetName()
{
	return "[VIP] Items";
}

const char *vip_items::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
