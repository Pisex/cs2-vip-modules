#include <stdio.h>
#include "vip_items.h"
#include "schemasystem/schemasystem.h"
#include <sstream>

vip_items g_vip_items;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

int iRoundMin;

PLUGIN_EXPOSE(vip_items, g_vip_items);
bool vip_items::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
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

std::string StripQuotes(const std::string& str) {
	if (str.length() >= 2 && str.front() == '"' && str.back() == '"') {
		return str.substr(1, str.length() - 2);
	}
	return str;
}

std::string TrimTrailingQuote(const std::string& str) {
	if (!str.empty() && str.back() == '"') {
		return str.substr(0, str.length() - 1);
	}
	return str;
}

std::vector<std::string> SplitStringBySpace(const std::string& input) {
	std::vector<std::string> tokens;
	std::string current;
	bool inQuotes = false;

	for (size_t i = 0; i < input.size(); ++i) {
		char ch = input[i];

		if (ch == '"') {
			inQuotes = !inQuotes;
			continue;
		}

		if (std::isspace(static_cast<unsigned char>(ch)) && !inQuotes) {
			if (!current.empty()) {
				tokens.push_back(current);
				current.clear();
			}
		} else {
			current += ch;
		}
	}

	if (!current.empty()) {
		tokens.push_back(current);
	}

	return tokens;
}

void VIP_OnPlayerSpawn(int iSlot, int iTeam, bool bIsVIP)
{
	if(bIsVIP)
	{
		if((iRoundMin == 1 && !g_pVIPCore->VIP_PistolRound()) || (iRoundMin <= g_pVIPCore->VIP_GetTotalRounds() && !g_pVIPCore->VIP_PistolRound()) || (iRoundMin == 0))
		{
			if(g_pVIPCore->VIP_GetClientFeatureString(iSlot, "items")[0])
			{
				CCSPlayerController* pPlayerController =  CCSPlayerController::FromSlot(iSlot);
				if(!pPlayerController) return;
				CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
				if (!pPlayerPawn || !pPlayerPawn->IsAlive()) return;
				CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
				if(!pItemServices) return;
				CPlayer_WeaponServices* pWeaponServices = pPlayerPawn->m_pWeaponServices();
				if(!pWeaponServices) return;
				std::vector<std::string> m_items;
				std::vector<std::string> hWeapons;
				CUtlVector<CHandle<CBasePlayerWeapon>>* weapons = pWeaponServices->m_hMyWeapons();
				FOR_EACH_VEC(*weapons, i)
				{
					CEntityInstance* hActiveWeapon = (CEntityInstance*)(*weapons)[i].Get();
					if(!hActiveWeapon) continue;
					hWeapons.push_back(std::string(hActiveWeapon->GetClassname()));
				}

				if (const char* pszItems = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "items"))
				{
					m_items = SplitStringBySpace(pszItems);
				}
				if (m_items.empty()) return;
				for (size_t i = 0; i < m_items.size(); ++i)
				{
					if(!strcmp(m_items[i].c_str(), "weapon_smokegrenade")) {
						if(!pWeaponServices->m_iAmmo()[15]) pItemServices->GiveNamedItem(m_items[i].c_str());
					} else if(!strcmp(m_items[i].c_str(), "weapon_flashbang")) {
						if(!pWeaponServices->m_iAmmo()[14]) pItemServices->GiveNamedItem(m_items[i].c_str());
					} else if(!strcmp(m_items[i].c_str(), "weapon_decoy")) {
						if(!pWeaponServices->m_iAmmo()[17]) pItemServices->GiveNamedItem(m_items[i].c_str());
					} else if(!strcmp(m_items[i].c_str(), "weapon_molotov")) {
						if(!pWeaponServices->m_iAmmo()[16]) pItemServices->GiveNamedItem(m_items[i].c_str());
					} else if(!strcmp(m_items[i].c_str(), "weapon_incgrenade")) {
						if(!pWeaponServices->m_iAmmo()[16]) pItemServices->GiveNamedItem(m_items[i].c_str());
					} else if(!strcmp(m_items[i].c_str(),"weapon_hegrenade")) {
						if(!pWeaponServices->m_iAmmo()[13]) pItemServices->GiveNamedItem(m_items[i].c_str());
					} else {
						auto it = std::find(hWeapons.begin(), hWeapons.end(), m_items[i]);
						if (it == hWeapons.end()) {
							pItemServices->GiveNamedItem(m_items[i].c_str());
						}
					}
				}
				hWeapons.clear();
			}
		}
	}
}

bool vip_items::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	delete g_pUtils;
	return true;
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pUtils->GetCGameEntitySystem();
}

void OnStartupServer()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
}

void vip_items::AllPluginsLoaded()
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
	g_pVIPCore->VIP_RegisterFeature("items", VIP_BOOL, TOGGLABLE);
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
