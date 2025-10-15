#include <stdio.h>
#include "vip_speed.h"
#include <sstream>
#include "schemasystem/schemasystem.h"

VIPSpeed g_VIPSpeed;

IUtilsApi* g_pUtils;
IVIPApi* g_pVIPCore;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(VIPSpeed, g_VIPSpeed);

bool VIPSpeed::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
    PLUGIN_SAVEVARS();
    GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
    GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
    GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);

    g_SMAPI->AddListener(this, this);
    return true;
}

void VIP_OnPlayerSpawn(int iSlot, int iTeam, bool bIsVIP)
{
    if (bIsVIP)
    {
        CCSPlayerController* pPlayerController = CCSPlayerController::FromSlot(iSlot);
        if (!pPlayerController) return;
        CCSPlayerPawn* pPlayerPawn = pPlayerController->m_hPlayerPawn();
        if (!pPlayerPawn || !pPlayerPawn->IsAlive()) return;

        const char* sSpeed = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "speed");
        if (strlen(sSpeed) > 0)
        {
            float fSpeed = atof(sSpeed);
            if (fSpeed >= 0.1f && fSpeed <= 2.0f)
            {
                pPlayerPawn->m_flVelocityModifier() = fSpeed;
            }
        }
    }
}

bool VIPSpeed::Unload(char* error, size_t maxlen)
{
    delete g_pVIPCore;
    delete g_pUtils;
    return true;
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pUtils->GetCGameEntitySystem();
}

void OnStartupServer()
{
    g_pGameEntitySystem = GameEntitySystem();
    g_pEntitySystem = g_pGameEntitySystem;
}

void VIPSpeed::AllPluginsLoaded()
{
    int ret;
    g_pUtils = (IUtilsApi*)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);
    if (ret == META_IFACE_FAILED)
    {
        char error[64];
        V_strncpy(error, "Failed to lookup utils api. Aborting", 64);
        ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
        std::string sBuffer = "meta unload " + std::to_string(g_PLID);
        engine->ServerCommand(sBuffer.c_str());
        return;
    }
    g_pVIPCore = (IVIPApi*)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
    if (ret == META_IFACE_FAILED)
    {
        g_pUtils->ErrorLog("[%s] Failed to lookup vip core. Aborting", GetLogTag());
        std::string sBuffer = "meta unload " + std::to_string(g_PLID);
        engine->ServerCommand(sBuffer.c_str());
        return;
    }
    g_pUtils->StartupServer(g_PLID, OnStartupServer);
    g_pVIPCore->VIP_OnPlayerSpawn(VIP_OnPlayerSpawn);
    g_pVIPCore->VIP_RegisterFeature("speed", VIP_BOOL, TOGGLABLE);
}

const char* VIPSpeed::GetLicense()
{
    return "Public";
}

const char* VIPSpeed::GetVersion()
{
    return "1.0";
}

const char* VIPSpeed::GetDate()
{
    return __DATE__;
}

const char* VIPSpeed::GetLogTag()
{
    return "[VIP-Speed]";
}

const char* VIPSpeed::GetAuthor()
{
    return "ABKAM";
}

const char* VIPSpeed::GetDescription()
{
    return "VIP Module for custom speed";
}

const char* VIPSpeed::GetName()
{
    return "[VIP] Speed";
}

const char* VIPSpeed::GetURL()
{
    return "https://discord.com/invite/g798xERK5Y";
}