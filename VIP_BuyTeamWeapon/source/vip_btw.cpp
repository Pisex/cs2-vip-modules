#include <stdio.h>
#include "vip_btw.h"
#include "schemasystem/schemasystem.h"
#include <sstream>

vip_btw g_vip_btw;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

CCSGameRules* g_pGameRules = nullptr;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

struct WeaponData
{
	const char* szName;
	int iPrice;
};

std::map<std::string, WeaponData> g_WeaponData[2];

PLUGIN_EXPOSE(vip_btw, g_vip_btw);
bool vip_btw::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );

	return true;
}

bool vip_btw::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	delete g_pUtils;
	return true;
}

void OnRoundStart(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
    for (int i = 0; i < 64; i++)
    {
        if(g_pVIPCore->VIP_IsClientVIP(i) && g_pVIPCore->VIP_GetClientFeatureBool(i, "btw"))
        {
            CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(i);
            if(!pPlayerController) return;
            CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
            if (!pPlayerPawn || !pPlayerPawn->IsAlive()) return;
			int teamIndex = pPlayerPawn->m_iTeamNum() - 2;
			if (teamIndex < 0 || teamIndex >= 2) return;

			std::string szWeapons;
			char szBuffer[64];
			for (const auto& pair : g_WeaponData[teamIndex])
			{
				if (!szWeapons.empty())
				{
					g_SMAPI->Format(szBuffer, sizeof(szBuffer), ", %s", pair.first.c_str());
					szWeapons += szBuffer;
				}
				else
				{
					szWeapons = pair.first.c_str();
				}
			}
			g_pUtils->PrintToChat(i, g_pVIPCore->VIP_GetTranslate("btw_round_start"), szWeapons.c_str());
        }
    }
}

bool VIP_BuyWeapon(int iSlot, const char* szContent)
{
	CCommand arg;
	arg.Tokenize(szContent);
	if(arg.ArgC() < 1) return false;
	std::string sCommand = arg.Arg(0);
	if(g_WeaponData[0].find(sCommand) == g_WeaponData[0].end() && g_WeaponData[1].find(sCommand) == g_WeaponData[1].end()) return false;
	if(!g_pVIPCore->VIP_IsClientVIP(iSlot) || !g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "btw")) return false;
	CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
	if(!pPlayerController) return false;
	CCSPlayerPawn* pPlayerPawn = pPlayerController->GetPlayerPawn();
	if(!pPlayerPawn || !pPlayerPawn->IsAlive()) return false;
	int iTeam = pPlayerPawn->m_iTeamNum();
	int iTeamNum = iTeam == 3 ? 1 : 0;
	if(g_WeaponData[iTeamNum].find(sCommand) == g_WeaponData[iTeamNum].end()) return false;
	if(!pPlayerPawn->m_bInBuyZone())
	{
		g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("btw_no_buyzone"));
		return false;
	}
	CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
	if(!pItemServices) return false;
	CCSPlayerController_InGameMoneyServices* pMoneyServices = pPlayerController->m_pInGameMoneyServices();
	if(!pMoneyServices) return false;

	WeaponData wData = g_WeaponData[iTeamNum][sCommand];
	if(pMoneyServices->m_iAccount() < wData.iPrice)
	{
		g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("btw_no_money"));
		return false;
	}
	
	pMoneyServices->m_iAccount() -= wData.iPrice;
	pItemServices->GiveNamedItem(wData.szName);
	g_pUtils->SetStateChanged(pPlayerController, "CCSPlayerController", "m_pInGameMoneyServices");
	return false;
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

void GetGameRules()
{
	g_pGameRules = g_pUtils->GetCCSGameRules();
}

void LoadConfig()
{
	KeyValues* hKv = new KeyValues("VIP");
	const char *pszPath = "addons/configs/vip/vip_btw.ini";

	if (!hKv->LoadFromFile(g_pFullFileSystem, pszPath))
	{
		g_pUtils->ErrorLog("[%s] Failed to load %s", g_PLAPI->GetLogTag(), pszPath);
		return;
	}

	KeyValues* hCT = hKv->FindKey("CT");
	if(hCT)
	{
		FOR_EACH_TRUE_SUBKEY(hCT, pValue)
		{
			const char* szCommand = pValue->GetName();
			int iPrice = pValue->GetInt("price");
			const char* szWeapon = pValue->GetString("weapon");
			g_WeaponData[1][szCommand] = {szWeapon, iPrice};
			g_pUtils->RegCommand(g_PLID, {}, {szCommand}, VIP_BuyWeapon);
		}
	}
	KeyValues* hT = hKv->FindKey("T");
	if(hT)
	{
		FOR_EACH_TRUE_SUBKEY(hT, pValue)
		{
			const char* szCommand = pValue->GetName();
			int iPrice = pValue->GetInt("price");
			const char* szWeapon = pValue->GetString("weapon");
			g_WeaponData[0][szCommand] = {szWeapon, iPrice};
			g_pUtils->RegCommand(g_PLID, {}, {szCommand}, VIP_BuyWeapon);
		}
	}
}

void vip_btw::AllPluginsLoaded()
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
	g_pUtils->OnGetGameRules(g_PLID, GetGameRules);
	g_pUtils->HookEvent(g_PLID, "round_start", OnRoundStart);
	g_pVIPCore->VIP_RegisterFeature("btw", VIP_BOOL, HIDE);
	LoadConfig();
}

const char *vip_btw::GetLicense()
{
	return "Public";
}

const char *vip_btw::GetVersion()
{
	return "1.0";
}

const char *vip_btw::GetDate()
{
	return __DATE__;
}

const char *vip_btw::GetLogTag()
{
	return "[VIP-BTW]";
}

const char *vip_btw::GetAuthor()
{
	return "Pisex";
}

const char *vip_btw::GetDescription()
{
	return "";
}

const char *vip_btw::GetName()
{
	return "[VIP] Buy Team Weapon";
}

const char *vip_btw::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
