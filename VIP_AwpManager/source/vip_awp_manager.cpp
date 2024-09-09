#include <stdio.h>
#include "vip_awp_manager.h"
#include "metamod_oslink.h"
#include "schemasystem/schemasystem.h"

vip_awp_manager g_vip_awp_manager;
PLUGIN_EXPOSE(vip_awp_manager, g_vip_awp_manager);
IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

IUtilsApi* g_pUtils;
IPlayersApi* g_pPlayers;
IVIPApi* g_pVIPCore;

CEntityListener g_EntityListener;

CGameEntitySystem* GameEntitySystem()
{
	return g_pUtils->GetCGameEntitySystem();
}

void StartupServer()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pUtils->GetCEntitySystem();
	g_pGameEntitySystem->AddListenerEntity(&g_EntityListener);
}

bool vip_awp_manager::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);

	g_SMAPI->AddListener( this, this );

	return true;
}

bool vip_awp_manager::Unload(char *error, size_t maxlen)
{
	g_pGameEntitySystem->RemoveListenerEntity(&g_EntityListener);
	ConVar_Unregister();
	
	return true;
}

CCSPlayerController* GetPlayer(uint32 iSteamID)
{
    for(int i = 0; i < 64; i++)
    {
        if(g_pPlayers->IsFakeClient(i)) continue;
        if(!g_pPlayers->IsAuthenticated(i)) continue;
        if(!g_pPlayers->IsConnected(i)) continue;
        if(!g_pPlayers->IsInGame(i)) continue;
        int iSteamID2 = g_pPlayers->GetSteamID64(i);
        if(iSteamID2 == iSteamID)
            return CCSPlayerController::FromSlot(i);
    }
    return nullptr;
}

void CEntityListener::OnEntitySpawned(CEntityInstance* pEntity)
{
	if(!pEntity) return;
	if (!strcmp(pEntity->GetClassname(), "weapon_awp"))
    {
        CBaseEntity* pBaseEntity = dynamic_cast<CBaseEntity*>(pEntity);
        if (!pBaseEntity) return;

        CHandle<CBaseEntity> hEnt = pBaseEntity->GetHandle();
        g_pUtils->NextFrame([hEnt]() {
            CBaseEntity* pEnt = hEnt.Get();
            if (pEnt)
            {
                CBasePlayerWeapon* pWeapon = dynamic_cast<CBasePlayerWeapon*>(pEnt);
                if (pWeapon)
                {
                    uint32 iSteamID = pWeapon->m_OriginalOwnerXuidLow();
					if(iSteamID == 0) return;
					CCSPlayerController* pPlayer = GetPlayer(iSteamID);
					if(!pPlayer) return;
					int iSlot = pPlayer->GetPlayerSlot();
					if(iSlot < 0 || iSlot > 63) return;
					if (g_pVIPCore->VIP_IsClientVIP(iSlot))
					{
						int iAmmo = g_pVIPCore->VIP_GetClientFeatureInt(iSlot, "awp_manager");
						if(iAmmo > 0)
						{
							if (pWeapon->GetWeaponVData())
							{
								pWeapon->GetWeaponVData()->m_iMaxClip1 = iAmmo;
								pWeapon->GetWeaponVData()->m_iDefaultClip1 = iAmmo;
							}
							pWeapon->m_iClip1 = iAmmo;
						}
						else
						{
							if (pWeapon->GetWeaponVData())
							{
								pWeapon->GetWeaponVData()->m_iMaxClip1 = 5;
								pWeapon->GetWeaponVData()->m_iDefaultClip1 = 5;
							}
							pWeapon->m_iClip1 = 5;
						}
                    }
					else
					{
						if (pWeapon->GetWeaponVData())
						{
							pWeapon->GetWeaponVData()->m_iMaxClip1 = 5;
							pWeapon->GetWeaponVData()->m_iDefaultClip1 = 5;
						}
						pWeapon->m_iClip1 = 5;
					}
                }
            }
		});
    }
}

void vip_awp_manager::AllPluginsLoaded()
{
	char error[64];
	int ret;
	g_pUtils = (IUtilsApi *)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_SMAPI->Format(error, sizeof(error), "Missing Utils system plugin");
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pPlayers = (IPlayersApi *)g_SMAPI->MetaFactory(PLAYERS_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_SMAPI->Format(error, sizeof(error), "Missing Players system plugin");
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pVIPCore = (IVIPApi *)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		g_SMAPI->Format(error, sizeof(error), "Missing VIP system plugin");
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}

	g_pVIPCore->VIP_RegisterFeature("awp_manager", VIP_INT, TOGGLABLE);
	g_pUtils->StartupServer(g_PLID, StartupServer);
}

///////////////////////////////////////
const char* vip_awp_manager::GetLicense()
{
	return "GPL";
}

const char* vip_awp_manager::GetVersion()
{
	return "1.0";
}

const char* vip_awp_manager::GetDate()
{
	return __DATE__;
}

const char *vip_awp_manager::GetLogTag()
{
	return "vip_awp_manager";
}

const char* vip_awp_manager::GetAuthor()
{
	return "Pisex";
}

const char* vip_awp_manager::GetDescription()
{
	return "vip_awp_manager";
}

const char* vip_awp_manager::GetName()
{
	return "[VIP] AWP Manager";
}

const char* vip_awp_manager::GetURL()
{
	return "https://discord.gg/g798xERK5Y";
}
