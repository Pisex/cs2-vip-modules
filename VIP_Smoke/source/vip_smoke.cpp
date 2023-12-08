#include <stdio.h>
#include "vip_smoke.h"

vip_smoke g_vip_smoke;

IVIPApi* g_pVIPCore;
CEntityListener g_EntityListener;

IVEngineServer2* engine = nullptr;
CSchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);

PLUGIN_EXPOSE(vip_smoke, g_vip_smoke);

bool vip_smoke::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	g_SMAPI->AddListener( this, this );
	SH_ADD_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_smoke::GameFrame), true);
	return true;
}

void vip_smoke::NextFrame(std::function<void()> fn)
{
	m_nextFrame.push_back(fn);
}

void vip_smoke::GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	while (!m_nextFrame.empty())
	{
		m_nextFrame.front()();
		m_nextFrame.pop_front();
	}
}

bool vip_smoke::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_smoke::GameFrame), true);
	g_pGameEntitySystem->RemoveListenerEntity(&g_EntityListener);
	delete g_pVIPCore;
	return true;
}

void VIP_OnVIPLoaded()
{
	g_pGameEntitySystem = g_pVIPCore->VIP_GetEntitySystem();
	g_pEntitySystem = g_pGameEntitySystem;
	g_pGameEntitySystem->AddListenerEntity(&g_EntityListener);
}

void CEntityListener::OnEntitySpawned(CEntityInstance* pEntity)
{
	CSmokeGrenadeProjectile* pGrenadeProjectile = dynamic_cast<CSmokeGrenadeProjectile*>(pEntity);
	if (!pGrenadeProjectile)
		return;

	g_vip_smoke.NextFrame([hGrenadeProjectile = CHandle<CSmokeGrenadeProjectile>(pGrenadeProjectile)]()
	{
		CSmokeGrenadeProjectile* pGrenadeProjectile = hGrenadeProjectile;
		if (!pGrenadeProjectile)
			return;

		CCSPlayerPawn* pPlayerPawn = pGrenadeProjectile->m_hThrower();
		if (!pPlayerPawn)
			return;
		int iSlot = pPlayerPawn->m_hController()->m_pEntity->m_EHandle.GetEntryIndex() - 1;
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
	g_pVIPCore->VIP_OnVIPLoaded(VIP_OnVIPLoaded);
}

const char *vip_smoke::GetLicense()
{
	return "Public";
}

const char *vip_smoke::GetVersion()
{
	return "1.0";
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
