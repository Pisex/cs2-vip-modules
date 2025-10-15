#include <stdio.h>
#include "vip_joinsound.h"

VIPJoinSound g_VIPJoinSound;

IVIPApi* g_pVIPCore;
IUtilsApi* g_pUtils;
IPlayersApi* g_pPlayers;

IVEngineServer2* engine = nullptr;
ISchemaSystem* g_pCSchemaSystem = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

PLUGIN_EXPOSE(VIPJoinSound, g_VIPJoinSound);

bool VIPJoinSound::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
    PLUGIN_SAVEVARS();
    GET_V_IFACE_ANY(GetEngineFactory, g_pCSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
    GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
    g_SMAPI->AddListener(this, this);
    return true;
}

bool VIPJoinSound::Unload(char *error, size_t maxlen)
{
    delete g_pVIPCore;
    g_pUtils->ClearAllHooks(g_PLID);
    delete g_pUtils;
    return true;
}
void OnPlayerConnectFull(const char* szName, IGameEvent* pEvent, bool bDontBroadcast)
{
    int iSlot = pEvent->GetInt("userid"); 
    const char* musicPath = g_pVIPCore->VIP_GetClientFeatureString(iSlot, "JoinSound");
    if (musicPath && *musicPath)
    {
        for (int i = 0; i < 64; ++i)
        {
            if (g_pPlayers->IsConnected(i) && !g_pPlayers->IsFakeClient(i))
            {
                char command[256];
                snprintf(command, sizeof(command), "play %s", musicPath);

                engine->ClientCommand(i, "%s", command);
            }
        }
    }
    else
    {
        ConColorMsg(Color(255, 0, 0, 255), "[VIP-JoinSound] No valid music path found for VIP player %d.\n", iSlot);
    }
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pVIPCore->VIP_GetEntitySystem();
};

void VIP_OnVIPLoaded()
{
    g_pGameEntitySystem = GameEntitySystem();
    g_pEntitySystem = g_pGameEntitySystem;
    g_pUtils->HookEvent(g_PLID, "player_connect_full", OnPlayerConnectFull);
}

void VIPJoinSound::AllPluginsLoaded()
{
    int ret;
    g_pVIPCore = (IVIPApi*)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);

    if (ret == META_IFACE_FAILED)
    {
        char error[64];
        V_strncpy(error, "Failed to lookup vip core. Aborting", 64);
        ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
        std::string sBuffer = "meta unload " + std::to_string(g_PLID);
        engine->ServerCommand(sBuffer.c_str());
        return;
    }
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
    g_pPlayers = (IPlayersApi*)g_SMAPI->MetaFactory(PLAYERS_INTERFACE, &ret, NULL);
    if (ret == META_IFACE_FAILED)
    {
        g_pUtils->ErrorLog("[%s] Failed to lookup players api. Aborting", GetLogTag());
        std::string sBuffer = "meta unload " + std::to_string(g_PLID);
        engine->ServerCommand(sBuffer.c_str());
        return;
    }

    g_pVIPCore->VIP_OnVIPLoaded(VIP_OnVIPLoaded);
    g_pVIPCore->VIP_RegisterFeature("JoinSound", VIP_BOOL, TOGGLABLE);
}

const char *VIPJoinSound::GetLicense()
{
    return "Public";
}

const char *VIPJoinSound::GetVersion()
{
    return "1.0";
}

const char *VIPJoinSound::GetDate()
{
    return __DATE__;
}

const char *VIPJoinSound::GetLogTag()
{
    return "[VIP-JoinSound]";
}

const char *VIPJoinSound::GetAuthor()
{
    return "ABKAM";
}

const char *VIPJoinSound::GetDescription()
{
    return "Plays a sound for all players when a VIP joins.";
}

const char *VIPJoinSound::GetName()
{
    return "[VIP] Join Sound";
}

const char *VIPJoinSound::GetURL()
{
    return "https://discord.com/invite/g798xERK5Y";
}
