#include <stdio.h>
#include "vip_smoke.h"

vip_smoke g_vip_smoke;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;

CEntityListener g_EntityListener;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(vip_smoke, g_vip_smoke);

bool vip_smoke::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	return true;
}

bool vip_smoke::Unload(char *error, size_t maxlen)
{
	g_pGameEntitySystem->RemoveListenerEntity(&g_EntityListener);
	delete g_pVIPCore;
	delete g_pUtils;
	return true;
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pVIPCore->VIP_GetEntitySystem();
};

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pGameEntitySystem->AddListenerEntity(&g_EntityListener);
}

void CEntityListener::OnEntitySpawned(CEntityInstance* pEntity)
{
	CSmokeGrenadeProjectile* pGrenadeProjectile = dynamic_cast<CSmokeGrenadeProjectile*>(pEntity);
	if (!pGrenadeProjectile)
		return;

	g_pUtils->NextFrame([hGrenadeProjectile = CHandle<CSmokeGrenadeProjectile>(pGrenadeProjectile)]()
	{
		CSmokeGrenadeProjectile* pGrenadeProjectile = hGrenadeProjectile.Get();
		if (!pGrenadeProjectile) return;

		CCSPlayerPawn* pPlayerPawn = pGrenadeProjectile->m_hThrower();
		if (!pPlayerPawn) return;
		if (!pPlayerPawn->IsAlive()) return;

		CBasePlayerController* pController = pPlayerPawn->GetController();
		if (!pController) return;

		int iSlot = pController->GetPlayerSlot();
		if(iSlot < 0 || iSlot > 63) return;
		
		if(!g_pVIPCore->VIP_IsClientVIP(iSlot))
			return;
			
		const char* pszSmokeColor = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "smoke_color");
		if(strlen(pszSmokeColor) > 0)
		{
			CCommand args;
			args.Tokenize(pszSmokeColor);
			pGrenadeProjectile->m_vSmokeColor() = !strcmp(pszSmokeColor, "random") ? Vector(rand() % 255, rand() % 255, rand() % 255) : Vector(std::stoi(args[0]), std::stoi(args[1]), std::stoi(args[2]));
		}
	});
}

void vip_smoke::AllPluginsLoaded()
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
		V_strncpy(error, "Failed to lookup utils core. Aborting", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}

	g_pVIPCore->VIP_OnVIPLoaded(VIP_OnVIPLoaded);
	g_pVIPCore->VIP_RegisterFeature("smoke_color", VIP_BOOL, TOGGLABLE);
}

const char *vip_smoke::GetLicense()
{
	return "Public";
}

const char *vip_smoke::GetVersion()
{
	return "1.0.1";
}

const char *vip_smoke::GetDate()
{
	return __DATE__;
}

const char *vip_smoke::GetLogTag()
{
	return "[VIP-SMOKE]";
}

const char *vip_smoke::GetAuthor()
{
	return "Pisex";
}

const char *vip_smoke::GetDescription()
{
	return "";
}

const char *vip_smoke::GetName()
{
	return "[VIP] Colored Smoke";
}

const char *vip_smoke::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
