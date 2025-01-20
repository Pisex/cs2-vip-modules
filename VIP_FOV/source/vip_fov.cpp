#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "vip_fov.h"
#include "schemasystem/schemasystem.h"

VIPFov g_VIPFov;

IVIPApi* g_pVIPCore;
IMenusApi* g_pMenus;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

std::vector<std::string> g_FOV[64];

PLUGIN_EXPOSE(VIPFov, g_VIPFov);
bool VIPFov::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool VIPFov::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	return true;
}

void VIP_OnClientLoaded(int iSlot, bool bIsVIP)
{
	g_FOV[iSlot].clear();
	if(bIsVIP)
	{	
		const char* szFOV = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "FOV");
		std::stringstream iss(szFOV);
		std::string token;
		
		while (std::getline(iss, token, ',')) {
			g_FOV[iSlot].push_back(token);
		}
	}
}

void VIP_OnPlayerSpawn(int iSlot, int iTeam, bool bIsVIP)
{
	if(bIsVIP)
	{
		CCSPlayerController* pController = CCSPlayerController::FromSlot(iSlot);
		if (!pController) return;
		
		if(g_FOV[iSlot].size() > 0)
		{
			const char* szFOV = g_pVIPCore->VIP_GetClientCookie(iSlot, "FOV_Value");
			int iFOV = szFOV[0]?std::stoi(szFOV):90;
			pController->m_iDesiredFOV() = iFOV;
			g_pUtils->SetStateChanged(pController, "CBasePlayerController", "m_iDesiredFOV");
		}
	}
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

bool OnSelect(int iSlot, const char* szFeature)
{
	Menu hMenu;
	g_pMenus->SetTitleMenu(hMenu, g_pVIPCore->VIP_GetTranslate("FOV_Title"));
	for (const auto& element : g_FOV[iSlot]) {
        g_pMenus->AddItemMenu(hMenu, element.c_str(), element.c_str());
    }
	g_pMenus->SetExitMenu(hMenu, true);
	g_pMenus->SetBackMenu(hMenu, true);
	g_pMenus->SetCallback(hMenu, [](const char* szBack, const char* szFront, int iItem, int iSlot)
	{
		if(iItem < 7)
		{
			CCSPlayerController* pController = CCSPlayerController::FromSlot(iSlot);
			if (!pController)
				return;
			int iFOV = std::stoi(szBack);
			g_pVIPCore->VIP_SetClientCookie(iSlot, "FOV_Value", strdup(szBack));
			pController->m_iDesiredFOV() = iFOV;
			g_pUtils->SetStateChanged(pController, "CBasePlayerController", "m_iDesiredFOV");
			OnSelect(iSlot, "FOV");
		}
		else if(iItem == 7) g_pVIPCore->VIP_OpenMenu(iSlot);
	});
	g_pMenus->DisplayPlayerMenu(hMenu, iSlot);
	return false;
}

void VIPFov::AllPluginsLoaded()
{
	char error[64] = { 0 };
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
	g_pVIPCore = (IVIPApi*)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_pUtils->ErrorLog("[%s] Failed to lookup vip core. Aborting", GetLogTag());
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}

	g_pUtils->StartupServer(g_PLID, OnStartupServer);
	g_pVIPCore->VIP_OnClientLoaded(VIP_OnClientLoaded);
	g_pVIPCore->VIP_OnPlayerSpawn(VIP_OnPlayerSpawn);
	g_pVIPCore->VIP_RegisterFeature("FOV", VIP_BOOL, SELECTABLE, OnSelect);
}

const char *VIPFov::GetLicense()
{
	return "Public";
}

const char *VIPFov::GetVersion()
{
	return "1.0";
}

const char *VIPFov::GetDate()
{
	return __DATE__;
}

const char *VIPFov::GetLogTag()
{
	return "[VIP-FOV]";
}

const char *VIPFov::GetAuthor()
{
	return "Pisex";
}

const char *VIPFov::GetDescription()
{
	return "";
}

const char *VIPFov::GetName()
{
	return "[VIP] FOV";
}

const char *VIPFov::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
