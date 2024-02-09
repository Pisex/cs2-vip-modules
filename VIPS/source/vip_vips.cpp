#include <stdio.h>
#include "vip_vips.h"

vip_vips g_vip_vips;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

int iType;

PLUGIN_EXPOSE(vip_vips, g_vip_vips);
bool vip_vips::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );

	{
		KeyValues* hKv = new KeyValues("VIP");
		const char *pszPath = "addons/configs/vip/vip_vips.ini";

		if (!hKv->LoadFromFile(g_pFullFileSystem, pszPath))
		{
			Warning("Failed to load %s\n", pszPath);
			return false;
		}

		iType = hKv->GetInt("type");
		delete hKv;
	}

	return true;
}

bool vip_vips::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	return true;
}

bool OnVipsCommand(int iSlot, const char* szContent)
{
	bool bFound = false;
	for (int i = 0; i < 64; i++)
	{
		if(g_pVIPCore->VIP_IsClientVIP(i))
		{
			CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(i + 1));
			if(!pPlayerController) continue;
			CCSPlayerPawn* pPlayerPawn = pPlayerController->m_hPlayerPawn();
			if(!pPlayerPawn) continue;
			if(!bFound)
			{
				g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("VIPS_Title"));
				bFound = true;
			}
			switch (iType)
			{
			case 0:
				g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("VIPS_Player"), pPlayerController->m_iszPlayerName(), g_pVIPCore->VIP_GetClientVIPGroup(i));
				break;
			case 1:
				g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("VIPS_Player2"), pPlayerController->m_iszPlayerName());
				break;
			}
		}
	}
	if(!bFound) g_pUtils->PrintToChat(iSlot, g_pVIPCore->VIP_GetTranslate("VIPS_NoPlayers"));
	return false;
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pVIPCore->GetCGameEntitySystem();
};

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pUtils->RegCommand(g_PLID, {"sm_vips", "mm_vips", "vips"}, {"!vips", "vips"}, OnVipsCommand);
}

void vip_vips::AllPluginsLoaded()
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

const char *vip_vips::GetLicense()
{
	return "Public";
}

const char *vip_vips::GetVersion()
{
	return "1.0";
}

const char *vip_vips::GetDate()
{
	return __DATE__;
}

const char *vip_vips::GetLogTag()
{
	return "[VIP-VIPS]";
}

const char *vip_vips::GetAuthor()
{
	return "Pisex";
}

const char *vip_vips::GetDescription()
{
	return "";
}

const char *vip_vips::GetName()
{
	return "[VIP] VIPS";
}

const char *vip_vips::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
