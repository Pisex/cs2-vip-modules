#include <stdio.h>
#include "vip_btw.h"

vip_btw g_vip_btw;

IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(vip_btw, g_vip_btw);
bool vip_btw::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_btw::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	return true;
}

void VIP_OnFireEvent(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
	if(!strcmp(szName, "round_start"))
	{
		for (int i = 0; i < 64; i++)
		{
			if(g_pVIPCore->VIP_IsClientVIP(i) && g_pVIPCore->VIP_GetClientFeatureBool(i, "btw"))
			{
				CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(i + 1));
				if(!pPlayerController) return;
				CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
				if (pPlayerPawn && pPlayerPawn->m_lifeState() == LIFE_ALIVE)
				{
					if(pPlayerPawn->m_iTeamNum() == 3)
						g_pVIPCore->VIP_PrintToChat(i, 3, "%s", g_pVIPCore->VIP_GetTranslate("btw_round_start1"));
					else if(pPlayerPawn->m_iTeamNum() == 2)
						g_pVIPCore->VIP_PrintToChat(i, 3, "%s", g_pVIPCore->VIP_GetTranslate("btw_round_start2"));
				}
			}
		}
	}
}

void VIP_AK47Command(const char* szContent, int iSlot)
{
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "btw"))
	{
		CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(iSlot + 1));
		if(!pPlayerController) return;
		CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
		if (!pPlayerPawn || pPlayerPawn->m_lifeState() != LIFE_ALIVE)
			return;
		CCSPlayerController_InGameMoneyServices* pMoneyServices = pPlayerController->m_pInGameMoneyServices();
		if(!pMoneyServices) return;
		if(pMoneyServices->m_iAccount() >= 2700)
		{
			CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
			pMoneyServices->m_iAccount() -=2700; 
			pItemServices->GiveNamedItem("weapon_ak47");
		}
		else g_pVIPCore->VIP_PrintToChat(iSlot, 3, "%s", g_pVIPCore->VIP_GetTranslate("btw_no_money"));
	}
	else g_pVIPCore->VIP_PrintToChat(iSlot, 3, "%s", g_pVIPCore->VIP_GetTranslate("NotAccess"));
}

void VIP_M4A1Command(const char* szContent, int iSlot)
{
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "btw"))
	{
		CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(iSlot + 1));
		if(!pPlayerController) return;
		CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
		if (!pPlayerPawn || pPlayerPawn->m_lifeState() != LIFE_ALIVE)
			return;
		CCSPlayerController_InGameMoneyServices* pMoneyServices = pPlayerController->m_pInGameMoneyServices();
		if(!pMoneyServices) return;
		if(pMoneyServices->m_iAccount() >= 2900)
		{
			CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
			pMoneyServices->m_iAccount() -=2900; 
			pItemServices->GiveNamedItem("weapon_m4a1_silencer");
		}
		else g_pVIPCore->VIP_PrintToChat(iSlot, 3, "%s", g_pVIPCore->VIP_GetTranslate("btw_no_money"));
	}
	else g_pVIPCore->VIP_PrintToChat(iSlot, 3, "%s", g_pVIPCore->VIP_GetTranslate("NotAccess"));
}

void VIP_M4A4Command(const char* szContent, int iSlot)
{
	if(g_pVIPCore->VIP_IsClientVIP(iSlot) && g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "btw"))
	{
		CCSPlayerController* pPlayerController =  (CCSPlayerController *)g_pEntitySystem->GetBaseEntity((CEntityIndex)(iSlot + 1));
		if(!pPlayerController) return;
		CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
		if (!pPlayerPawn || pPlayerPawn->m_lifeState() != LIFE_ALIVE)
			return;
		CCSPlayerController_InGameMoneyServices* pMoneyServices = pPlayerController->m_pInGameMoneyServices();
		if(!pMoneyServices) return;
		if(pMoneyServices->m_iAccount() >= 3100)
		{
			CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
			pMoneyServices->m_iAccount() -=3100; 
			pItemServices->GiveNamedItem("weapon_m4a1");
		}
		else g_pVIPCore->VIP_PrintToChat(iSlot, 3, "%s", g_pVIPCore->VIP_GetTranslate("btw_no_money"));
	}
	else g_pVIPCore->VIP_PrintToChat(iSlot, 3, "%s", g_pVIPCore->VIP_GetTranslate("NotAccess"));
}

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = g_pVIPCore->VIP_GetEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pVIPCore->VIP_OnFireEvent(VIP_OnFireEvent);
	g_pVIPCore->VIP_RegCommand("ak47", VIP_AK47Command);
	g_pVIPCore->VIP_RegCommand("m4a1", VIP_M4A1Command);
	g_pVIPCore->VIP_RegCommand("m4a4", VIP_M4A4Command);
}

void vip_btw::AllPluginsLoaded()
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
