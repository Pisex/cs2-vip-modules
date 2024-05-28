#include <stdio.h>
#include "vip_skins.h"
#include <fstream>
#include "entitykeyvalues.h"
#include <sstream>

vip_skins g_vip_skins;

IUtilsApi* g_pUtils;
IMenusApi* g_pMenus;
IVIPApi* g_pVIPCore;

float g_flUniversalTime;
float g_flLastTickedTime;
bool g_bHasTicked;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(vip_skins, g_vip_skins);

SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);

void (*UTIL_SetModel)(CBaseModelEntity*, const char* szModel) = nullptr;

struct Skin
{
	std::string sName;
	std::string sModel;
};

std::map<std::string, Skin> g_SkinsList;

std::string g_PlayerSkin[64];
std::string g_DefaultSkin[64];

float g_fTime;

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
	FOR_EACH_SUBKEY(g_kvSettings, pValue)
	{
		g_SkinsList[pValue->GetName()] = {pValue->GetString("name"), pValue->GetString("model")};
	}
}

void StartupServer()
{
	g_bHasTicked = false;
	LoadConfig();
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pUtils->GetCEntitySystem();
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

	SH_ADD_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_skins::GameFrame), true);

	DynLibUtils::CModule libserver(g_pSource2Server);
	UTIL_SetModel = libserver.FindPattern("55 48 89 F2 48 89 E5 41 54 49 89 FC 48 8D 7D E0 48 83 EC 18 48 8D 05 ? ? ? ? 48 8B 30 48 8B 06").RCast< decltype(UTIL_SetModel) >();
	if (!UTIL_SetModel)
	{
		V_strncpy(error, "Failed to find function to get UTIL_SetModel", maxlen);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);

		return false;
	}

	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_skins::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_skins::GameFrame), true);
	return true;
}

void vip_skins::GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	if (simulating && g_bHasTicked)
	{
		g_flUniversalTime += g_pUtils->GetCGlobalVars()->curtime - g_flLastTickedTime;
	}

	g_flLastTickedTime = g_pUtils->GetCGlobalVars()->curtime;
	g_bHasTicked = true;

	for (int i = g_timers.Tail(); i != g_timers.InvalidIndex();)
	{
		auto timer = g_timers[i];

		int prevIndex = i;
		i = g_timers.Previous(i);

		if (timer->m_flLastExecute == -1)
			timer->m_flLastExecute = g_flUniversalTime;

		// Timer execute 
		if (timer->m_flLastExecute + timer->m_flInterval <= g_flUniversalTime)
		{
			if (!timer->Execute())
			{
				delete timer;
				g_timers.Remove(prevIndex);
			}
			else
			{
				timer->m_flLastExecute = g_flUniversalTime;
			}
		}
	}
}

void OnPlayerSpawn(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	int iSlot = pEvent->GetInt("userid");
	if(iSlot < 0 || iSlot > 64)
		return;
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && !g_PlayerSkin[iSlot].empty())
	{
		auto pController = CCSPlayerController::FromSlot(iSlot);
		if (!pController)
			return;
		if(pController->m_iTeamNum() < 2 || !pController->IsAlive())
			return;
		g_DefaultSkin[iSlot] = pController->m_hPawn()->GetModelName();
		new CTimer(g_fTime, [pController, iSlot]() -> float {
			UTIL_SetModel(pController->m_hPawn().Get(), g_SkinsList[g_PlayerSkin[iSlot]].sModel.c_str());
			return -1.0f;
		});
	}
}
void SkinsMenuHandle(const char* szBack, const char* szFront, int iItem, int iSlot)
{
	if(iItem < 7)
	{
		auto pController = CCSPlayerController::FromSlot(iSlot);
		if (!pController)
			return;
		uint64_t m_steamID = pController->m_steamID();
		if(m_steamID == 0 || !pController->m_hPawn().Get())
			return;
		
		if(pController->m_iTeamNum() < 2 || !pController->IsAlive())
			return;
		
		g_PlayerSkin[iSlot] = szBack;
		g_pVIPCore->VIP_SetClientCookie(iSlot, "skin", szBack);
		if(!szBack[0])
			UTIL_SetModel(pController->m_hPawn().Get(), g_DefaultSkin[iSlot].c_str());
		else
		{
			g_DefaultSkin[iSlot] = pController->m_hPawn()->GetModelName();
			UTIL_SetModel(pController->m_hPawn().Get(), g_SkinsList[szBack].sModel.c_str());
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
	if(bIsVIP)
	{
		std::string skin = g_pVIPCore->VIP_GetClientCookie(iSlot, "skin");
		if(g_SkinsList.find(skin) != g_SkinsList.end())
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
