#include <stdio.h>
#include "vip_btw.h"
#include "schemasystem/schemasystem.h"

vip_btw g_vip_btw;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(vip_btw, g_vip_btw);
bool vip_btw::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
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
			if (pPlayerPawn && pPlayerPawn->IsAlive())
			{
				if(pPlayerPawn->m_iTeamNum() == 3)
					g_pUtils->PrintToChat(i, "%s", g_pVIPCore->VIP_GetTranslate("btw_round_start1"));
				else if(pPlayerPawn->m_iTeamNum() == 2)
					g_pUtils->PrintToChat(i, "%s", g_pVIPCore->VIP_GetTranslate("btw_round_start2"));
			}
		}
	}
}

bool VIP_AK47Command(int iSlot, const char* szContent)
{
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "btw"))
	{
		CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
		if(!pPlayerController) return false;
		CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
		if (!pPlayerPawn || !pPlayerPawn->IsAlive()) return false;
		CCSPlayerController_InGameMoneyServices* pMoneyServices = pPlayerController->m_pInGameMoneyServices();
		if(!pMoneyServices) return false;
		if(pMoneyServices->m_iAccount() >= 2700)
		{
			CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
			if(!pItemServices) return false;
			pMoneyServices->m_iAccount() -=2700; 
			pItemServices->GiveNamedItem("weapon_ak47");
			g_pUtils->SetStateChanged(pPlayerController, "CCSPlayerController", "m_pInGameMoneyServices");
		}
		else g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("btw_no_money"));
	}
	else g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("NotAccess"));
	return false;
}

bool VIP_M4A1Command(int iSlot, const char* szContent)
{
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "btw"))
	{
		CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
		if(!pPlayerController) return false;
		CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
		if (!pPlayerPawn || !pPlayerPawn->IsAlive()) return false;
		CCSPlayerController_InGameMoneyServices* pMoneyServices = pPlayerController->m_pInGameMoneyServices();
		if(!pMoneyServices) return false;
		if(pMoneyServices->m_iAccount() >= 2900)
		{
			CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
			if(!pItemServices) return false;
			pMoneyServices->m_iAccount() -=2900; 
			pItemServices->GiveNamedItem("weapon_m4a1_silencer");
			g_pUtils->SetStateChanged(pPlayerController, "CCSPlayerController", "m_pInGameMoneyServices");
		}
		else g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("btw_no_money"));
	}
	else g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("NotAccess"));
	return false;
}

bool VIP_M4A4Command(int iSlot, const char* szContent)
{
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "btw"))
	{
		CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
		if(!pPlayerController) return false;
		CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
		if (!pPlayerPawn || !pPlayerPawn->IsAlive()) return false;
		CCSPlayerController_InGameMoneyServices* pMoneyServices = pPlayerController->m_pInGameMoneyServices();
		if(!pMoneyServices) return false;
		if(pMoneyServices->m_iAccount() >= 3100)
		{
			CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
			if(!pItemServices) return false;
			pMoneyServices->m_iAccount() -=3100; 
			pItemServices->GiveNamedItem("weapon_m4a1");
			g_pUtils->SetStateChanged(pPlayerController, "CCSPlayerController", "m_pInGameMoneyServices");
		}
		else g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("btw_no_money"));
	}
	else g_pUtils->PrintToChat(iSlot, "%s", g_pVIPCore->VIP_GetTranslate("NotAccess"));
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
	g_pUtils->HookEvent(g_PLID, "round_start", OnRoundStart);
	g_pUtils->RegCommand(g_PLID, {"sm_ak47", "mm_ak47"}, {"!ak47", "ak47"}, VIP_AK47Command);
	g_pUtils->RegCommand(g_PLID, {"sm_m4a1", "mm_m4a1"}, {"!m4a1", "m4a1"}, VIP_M4A1Command);
	g_pUtils->RegCommand(g_PLID, {"sm_m4a4", "mm_m4a4"}, {"!m4a4", "m4a4"}, VIP_M4A4Command);
	g_pVIPCore->VIP_RegisterFeature("btw", VIP_BOOL, HIDE);
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
