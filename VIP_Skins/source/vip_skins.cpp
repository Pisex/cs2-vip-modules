#include <stdio.h>
#include "vip_skins.h"
#include <fstream>
#include <sstream>

vip_skins g_vip_skins;

IUtilsApi* g_pUtils;
IMenusApi* g_pMenus;
IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(vip_skins, g_vip_skins);

struct Skin
{
	std::string sName;
	std::string sModel_T;
	std::string sModel_CT;
};

std::map<std::string, Skin> g_SkinsList;

std::string g_PlayerSkin[64];
std::string g_DefaultSkin_CT[64];
std::string g_DefaultSkin_T[64];

int g_iCoolDown[64];
float g_fTime;
int g_iCoolD;

CGameEntitySystem* GameEntitySystem()
{
	return g_pUtils->GetCGameEntitySystem();
};

void LoadConfig()
{
	KeyValues* g_kvSettings = new KeyValues("Skins");
	char szPath[256];
	g_SMAPI->Format(szPath, sizeof(szPath), "addons/configs/vip/vip_skins.ini");

	if (!g_kvSettings->LoadFromFile(g_pFullFileSystem, szPath))
	{
		return;
	}
	g_SkinsList.clear();

	g_fTime = g_kvSettings->GetFloat("time", 2);
	g_iCoolD = g_kvSettings->GetInt("cooldown", 30);
	FOR_EACH_SUBKEY(g_kvSettings, pValue)
	{
		g_SkinsList[pValue->GetName()] = {pValue->GetString("name"), pValue->GetString("model_ct"), pValue->GetString("model_t")};
	}
}

void StartupServer()
{
	LoadConfig();
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
}

bool vip_skins::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetServerFactory, g_pSource2GameClients, IServerGameClients, SOURCE2GAMECLIENTS_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	ConVar_Register(FCVAR_RELEASE | FCVAR_CLIENT_CAN_EXECUTE | FCVAR_GAMEDLL);

	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_skins::Unload(char *error, size_t maxlen)
{
	return true;
}

void OnPlayerSpawn(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	int iSlot = pEvent->GetInt("userid");
	if(iSlot < 0 || iSlot > 64)
		return;
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && !g_PlayerSkin[iSlot].empty())
	{
		std::string sSkins = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "Skins");
		if(sSkins.find(g_PlayerSkin[iSlot]) == std::string::npos)
		{
			g_PlayerSkin[iSlot] = "";
			return;
		}
		g_pUtils->CreateTimer(g_fTime, [iSlot]() {
			auto pController = CCSPlayerController::FromSlot(iSlot);
			if (!pController) return -1.0f;
			CCSPlayerPawn* pPawn = pController->GetPlayerPawn();
			if(!pPawn) return -1.0f;
			if(pPawn->GetTeam() < 2 || !pPawn->IsAlive()) return -1.0f;
			if(pPawn->GetTeam() == 2) {
				g_DefaultSkin_T[iSlot] = pPawn->GetModelName().String();
				if(g_SkinsList[g_PlayerSkin[iSlot]].sModel_T.size())
					g_pUtils->SetEntityModel(pPawn, g_SkinsList[g_PlayerSkin[iSlot]].sModel_T.c_str());
			} else if(pPawn->GetTeam() == 3) {
				g_DefaultSkin_CT[iSlot] = pPawn->GetModelName().String();
				if(g_SkinsList[g_PlayerSkin[iSlot]].sModel_CT.size())
					g_pUtils->SetEntityModel(pPawn, g_SkinsList[g_PlayerSkin[iSlot]].sModel_CT.c_str());
			}
			return -1.0f;
		});
	}
}
void SkinsMenuHandle(const char* szBack, const char* szFront, int iItem, int iSlot)
{
	if(iItem < 7)
	{
		if(g_iCoolDown[iSlot] > std::time(0)) {
			g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("Cooldown"), g_iCoolDown[iSlot]-std::time(0));
			return;
		}
		g_iCoolDown[iSlot] = std::time(0) + g_iCoolD;
		g_PlayerSkin[iSlot] = szBack;
		CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
		if(!pPlayerController) return;
		CCSPlayerPawnBase* pPlayerPawn = pPlayerController->GetPlayerPawn();
		if(!pPlayerPawn) return;
		int iTeam = pPlayerPawn->GetTeam();
		if(iTeam < 2) return;
		if(!pPlayerPawn->IsAlive()) return;
		g_pVIPCore->VIP_SetClientCookie(iSlot, "skin", szBack);
		if(szBack[0] == '\0')
		{
			if(iTeam == 2)
			{
				if(g_DefaultSkin_T[iSlot].size())
					g_pUtils->SetEntityModel(pPlayerPawn, g_DefaultSkin_T[iSlot].c_str());
			}
			else if(iTeam == 3)
			{
				if(g_DefaultSkin_CT[iSlot].size())
					g_pUtils->SetEntityModel(pPlayerPawn, g_DefaultSkin_CT[iSlot].c_str());
			}
		}
		else
		{
			if(iTeam == 2)
			{
				g_DefaultSkin_T[iSlot] = pPlayerPawn->GetModelName().String();
				if(g_SkinsList[g_PlayerSkin[iSlot]].sModel_T.size())
					g_pUtils->SetEntityModel(pPlayerPawn, g_SkinsList[g_PlayerSkin[iSlot]].sModel_T.c_str());
			}
			else if(iTeam == 3)
			{
				g_DefaultSkin_CT[iSlot] = pPlayerPawn->GetModelName().String();
				if(g_SkinsList[g_PlayerSkin[iSlot]].sModel_CT.size())
					g_pUtils->SetEntityModel(pPlayerPawn, g_SkinsList[g_PlayerSkin[iSlot]].sModel_CT.c_str());
			}
		}
	}
}

bool OnSelect(int iSlot, const char* szFeature)
{
	Menu hMenu;
	g_pMenus->SetTitleMenu(hMenu, g_pVIPCore->VIP_GetTranslate("Select_Skin"));
	char szBuffer[64];
	std::string sSkins = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "Skins");
	g_pMenus->AddItemMenu(hMenu, "", g_pVIPCore->VIP_GetTranslate("Disable_Skin"));
	for(const auto& [key, value] : g_SkinsList)
	{
		if(sSkins.find(key) != std::string::npos)
			g_pMenus->AddItemMenu(hMenu, key.c_str(), value.sName.c_str());
	}
	g_pMenus->SetExitMenu(hMenu, true);
	g_pMenus->SetBackMenu(hMenu, false);
	g_pMenus->SetCallback(hMenu, SkinsMenuHandle);
	g_pMenus->DisplayPlayerMenu(hMenu, iSlot);
	return false;
}

void OnClientLoaded(int iSlot, bool bIsVIP)
{
	g_PlayerSkin[iSlot] = "";
	g_DefaultSkin_CT[iSlot] = "";
	g_DefaultSkin_T[iSlot] = "";
	if(bIsVIP)
	{
		std::string skin = g_pVIPCore->VIP_GetClientCookie(iSlot, "skin");
		std::string sSkins = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "Skins");
		if(g_SkinsList.find(skin) != g_SkinsList.end() && sSkins.find(skin) != std::string::npos)
			g_PlayerSkin[iSlot] = skin;
	}
}

void vip_skins::AllPluginsLoaded()
{
	char error[64];
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
	g_pUtils = (IUtilsApi*)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		V_strncpy(error, "Failed to lookup menus core. Aborting", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pMenus = (IMenusApi*)g_SMAPI->MetaFactory(Menus_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		char error[64];
		V_strncpy(error, "Failed to lookup menus api. Aborting", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pUtils->StartupServer(g_PLID, StartupServer);
	g_pUtils->HookEvent(g_PLID, "player_spawn", OnPlayerSpawn);
	g_pVIPCore->VIP_OnClientLoaded(OnClientLoaded);
	g_pVIPCore->VIP_RegisterFeature("Skins", VIP_BOOL, SELECTABLE, OnSelect);
}

const char *vip_skins::GetLicense()
{
	return "Public";
}

const char *vip_skins::GetVersion()
{
	return "1.0";
}

const char *vip_skins::GetDate()
{
	return __DATE__;
}

const char *vip_skins::GetLogTag()
{
	return "[vip_skins]";
}

const char *vip_skins::GetAuthor()
{
	return "Pisex";
}

const char *vip_skins::GetDescription()
{
	return "";
}

const char *vip_skins::GetName()
{
	return "[VIP] Skins";
}

const char *vip_skins::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
