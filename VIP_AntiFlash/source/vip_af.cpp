#include <stdio.h>
#include "vip_af.h"

vip_af g_vip_af;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(vip_af, g_vip_af);
bool vip_af::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_af::Unload(char *error, size_t maxlen)
{
	delete g_pVIPCore;
	g_pUtils->ClearAllHooks(g_PLID);
	delete g_pUtils;
	return true;
}

void OnPlayerBlind(const char* szName, IGameEvent* event, bool bDontBroadcast)
{
	CPlayerSlot pPlayerSlot = event->GetPlayerSlot("userid");
	if(g_pVIPCore->VIP_IsClientVIP(pPlayerSlot.Get()))
	{
		CCSPlayerController* pPlayerController = static_cast<CCSPlayerController*>(event->GetPlayerController("userid"));
		if (!pPlayerController)
			return;

		CCSPlayerController* pPlayerController_attacker = static_cast<CCSPlayerController*>(event->GetPlayerController("attacker"));
		if (!pPlayerController_attacker)
			return;

		CCSPlayerPawnBase* pPlayerPawn = pPlayerController->m_hPlayerPawn();
		if (!pPlayerPawn || pPlayerPawn->m_lifeState() != LIFE_ALIVE)
			return;

		CCSPlayer_ItemServices* pItemServices = static_cast<CCSPlayer_ItemServices*>(pPlayerPawn->m_pItemServices());
		int iAntiFlash = g_pVIPCore->VIP_GetClientFeatureInt(pPlayerSlot.Get(), "antiflash");
		if(iAntiFlash > 0)
		{
			int iATeam = pPlayerController_attacker->m_hPlayerPawn()->m_iTeamNum();
			int iTTeam = pPlayerController->m_hPlayerPawn()->m_iTeamNum();
			switch (iAntiFlash)
			{
				case 1:
				{
					if(iATeam == iTTeam && pPlayerController != pPlayerController_attacker) pPlayerPawn->m_flFlashDuration() = 0.0;
					break;
				}
				case 2:
				{
					if(pPlayerController == pPlayerController_attacker) pPlayerPawn->m_flFlashDuration() = 0.0;
					break;
				}
				case 3:
				{
					if(iATeam == iTTeam || pPlayerController == pPlayerController_attacker) pPlayerPawn->m_flFlashDuration() = 0.0;
					break;
				}
				case 4:
				{
					pPlayerPawn->m_flFlashDuration() = 0.0;
					break;
				}
			}
		}
	}
}

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = g_pVIPCore->VIP_GetEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
}

void vip_af::AllPluginsLoaded()
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
	g_pUtils->HookEvent(g_PLID, "player_blind", OnPlayerBlind);
	g_pVIPCore->VIP_OnVIPLoaded(VIP_OnVIPLoaded);
	g_pVIPCore->VIP_RegisterFeature("antiflash", VIP_BOOL, TOGGLABLE);
}

const char *vip_af::GetLicense()
{
	return "Public";
}

const char *vip_af::GetVersion()
{
	return "1.0";
}

const char *vip_af::GetDate()
{
	return __DATE__;
}

const char *vip_af::GetLogTag()
{
	return "[VIP-AF]";
}

const char *vip_af::GetAuthor()
{
	return "Pisex";
}

const char *vip_af::GetDescription()
{
	return "";
}

const char *vip_af::GetName()
{
	return "[VIP] Anti Flash";
}

const char *vip_af::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
