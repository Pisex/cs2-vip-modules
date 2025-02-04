#include <stdio.h>
#include "vip_test.h"

vip_test g_vip_test;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;
IMenusApi* g_pMenus;

IVEngineServer2* engine = nullptr;

int g_iType;

struct VIPData
{
	int iTime;
	int iTimeout;
	const char* szGroup;
	const char* szDisplay;
};
std::map<std::string, VIPData> g_VIPData;

PLUGIN_EXPOSE(vip_test, g_vip_test);

bool OnVIPTestCommand(int iSlot, const char* szContent)
{
	// if(!g_pVIPCore->VIP_IsClientVIP(iSlot))
	// {
	// 	const char* szValue = g_pVIPCore->VIP_GetClientCookie(iSlot, "vip_test");
	// 	if(!strlen(szValue) || ((std::stoi(szValue) < std::time(0)) && iTimeout != 0))
	// 	{
	// 		g_pVIPCore->VIP_SetClientCookie(iSlot, "vip_test", std::to_string(std::time(0)+iTime+iTimeout).c_str());
	// 		g_pVIPCore->VIP_GiveClientVIP(iSlot, iTime, szGroup);
	// 	}
	// 	else
	// 	{
	// 		time_t currentTime_t = (time_t)std::stoi(szValue);
	// 		char buffer[80];
	// 		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime_t));
	// 		g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate(iTimeout == 0?"ReceivedDisabled":"AlreadyReceived"), buffer);
	// 	}
	// }
	// else g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("AlreadyVIP"));
	Menu hMenu;
	g_pMenus->SetTitleMenu(hMenu, g_pVIPCore->VIP_GetTranslate("VIPTest_Title"));
	for(auto& it : g_VIPData)
	{
		g_pMenus->AddItemMenu(hMenu, it.second.szGroup, it.second.szDisplay);
	}
	g_pMenus->SetExitMenu(hMenu, true);
	g_pMenus->SetBackMenu(hMenu, false);
	g_pMenus->SetCallback(hMenu, [](const char* szBack, const char* szFront, int iItem, int iSlot)
	{
		if(iItem < 7) {
			if(!g_pVIPCore->VIP_IsClientVIP(iSlot)) {
				VIPData Group = g_VIPData[szBack];
				const char* szValue;
				if(g_iType == 1) {
					szValue = g_pVIPCore->VIP_GetClientCookie(iSlot, "vip_test");
				} else {
					char szBuffer[128];
					g_SMAPI->Format(szBuffer, sizeof(szBuffer), "vip_test_%s", Group.szGroup);
					szValue = g_pVIPCore->VIP_GetClientCookie(iSlot, szBuffer);
				}

				if(!strlen(szValue) || ((std::stoi(szValue) < std::time(0)) && Group.iTimeout != 0)) {
					if(g_iType == 1) {
						g_pVIPCore->VIP_SetClientCookie(iSlot, "vip_test", std::to_string(std::time(0)+Group.iTime+Group.iTimeout).c_str());
					} else {
						char szBuffer[128];
						g_SMAPI->Format(szBuffer, sizeof(szBuffer), "vip_test_%s", Group.szGroup);
						g_pVIPCore->VIP_SetClientCookie(iSlot, szBuffer, std::to_string(std::time(0)+Group.iTime+Group.iTimeout).c_str());
					}
					g_pUtils->NextFrame([iSlot, Group]() {g_pVIPCore->VIP_GiveClientVIP(iSlot, Group.iTime, Group.szGroup);});
				} else {
					time_t currentTime_t = (time_t)std::stoi(szValue);
					char buffer[80];
					std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime_t));
					g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate(Group.iTimeout == 0?"ReceivedDisabled":"AlreadyReceived"), buffer);
				}
			} else {
				g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("AlreadyVIP"));
			}
		}
	});
	g_pMenus->DisplayPlayerMenu(hMenu, iSlot, true, true);
	return false;
}

bool vip_test::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );

	return true;
}

bool vip_test::Unload(char *error, size_t maxlen)
{
	return true;
}

void LoadConfig()
{
	KeyValues* hKv = new KeyValues("VIP");
	const char *pszPath = "addons/configs/vip/vip_test.ini";

	if (!hKv->LoadFromFile(g_pFullFileSystem, pszPath))
	{
		g_pUtils->ErrorLog("[%s] Failed to load %s\n", g_PLAPI->GetLogTag(), pszPath);
		return;
	}

	g_iType = hKv->GetInt("type", 2);

	FOR_EACH_TRUE_SUBKEY(hKv, pValue)
	{
		const char* szGroup = pValue->GetName();
		const char* szDisplay = pValue->GetString("display", "");
		int iTime = pValue->GetInt("time", 0);
		int iTimeout = pValue->GetInt("timeout", 0);
		g_VIPData[szGroup] = {iTime, iTimeout, szGroup, szDisplay};
	}
}

void vip_test::AllPluginsLoaded()
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

	LoadConfig();
	g_pUtils->RegCommand(g_PLID, {"mm_testvip", "sm_testvip", "mm_viptest", "sm_viptest"}, {"!testvip", "testvip", "!viptest", "viptest"}, OnVIPTestCommand);
}

const char *vip_test::GetLicense()
{
	return "Public";
}

const char *vip_test::GetVersion()
{
	return "1.0";
}

const char *vip_test::GetDate()
{
	return __DATE__;
}

const char *vip_test::GetLogTag()
{
	return "[VIP-TEST]";
}

const char *vip_test::GetAuthor()
{
	return "Pisex";
}

const char *vip_test::GetDescription()
{
	return "";
}

const char *vip_test::GetName()
{
	return "[VIP] Multi Test";
}

const char *vip_test::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
