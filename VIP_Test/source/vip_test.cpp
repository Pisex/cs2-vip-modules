#include <stdio.h>
#include "vip_test.h"

vip_test g_vip_test;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;

char szGroup[128];
int iTime;
int iTimeout;

PLUGIN_EXPOSE(vip_test, g_vip_test);

bool OnVIPTestCommand(int iSlot, const char* szContent)
{
	if(!g_pVIPCore->VIP_IsClientVIP(iSlot))
	{
		const char* szValue = g_pVIPCore->VIP_GetClientCookie(iSlot, "vip_test");
		if(!strlen(szValue) || ((std::stoi(szValue) < std::time(0)) && iTimeout != 0))
		{
			g_pVIPCore->VIP_SetClientCookie(iSlot, "vip_test", std::to_string(std::time(0)+iTime+iTimeout).c_str());
			g_pVIPCore->VIP_GiveClientVIP(iSlot, iTime, szGroup);
		}
		else
		{
			time_t currentTime_t = (time_t)std::stoi(szValue);
			char buffer[80];
			std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime_t));
			g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate(iTimeout == 0?"ReceivedDisabled":"AlreadyReceived"), buffer);
		}
	}
	else g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("AlreadyVIP"));
	return false;
}

bool vip_test::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );

	{
		KeyValues* hKv = new KeyValues("VIP");
		const char *pszPath = "addons/configs/vip/vip_test.ini";

		if (!hKv->LoadFromFile(g_pFullFileSystem, pszPath))
		{
			Warning("Failed to load %s\n", pszPath);
			return false;
		}

		g_SMAPI->Format(szGroup, sizeof(szGroup), hKv->GetString("group"));
		iTime = hKv->GetInt("time");
		iTimeout = hKv->GetInt("timeout");
		delete hKv;
	}
	return true;
}

bool vip_test::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	return true;
}

void VIP_OnVIPLoaded()
{
	g_pUtils->RegCommand(g_PLID, {"mm_viptest", "sm_viptest"}, {"!viptest", "viptest"}, OnVIPTestCommand);
	g_pUtils->RegCommand(g_PLID, {"mm_testvip", "sm_testvip"}, {"!testvip", "testvip"}, OnVIPTestCommand);
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
	g_pVIPCore->VIP_OnVIPLoaded(VIP_OnVIPLoaded);
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
	return "[VIP] VIP Test";
}

const char *vip_test::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
