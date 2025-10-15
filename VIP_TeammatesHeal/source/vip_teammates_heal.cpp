#include <stdio.h>
#include "vip_teammates_heal.h"
#include <sstream>
#include "schemasystem/schemasystem.h"

HealModule g_HealModule;

IUtilsApi* g_pUtils = nullptr;
IVIPApi* g_pVIPCore = nullptr;
IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

bool g_bCanHeal[64];
float fEffectTime = 1.2f;

const char* g_szWeaponBlackList;
int g_iMaxShotHP;

bool g_bSyringeEffectEnabled = true;

PLUGIN_EXPOSE(HealModule, g_HealModule);

bool HealModule::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
    PLUGIN_SAVEVARS();
    GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
    GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
    GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);

    g_SMAPI->AddListener(this, this);

    {
        KeyValues* pKV = new KeyValues("VIP_Teammates_Heal");
        const char* pszPath = "addons/configs/vip/teammates_heal.ini";

        if (!pKV->LoadFromFile(g_pFullFileSystem, pszPath))
        {
            Warning("Failed to load %s\n", pszPath);
            return false;
        }

        g_bSyringeEffectEnabled = pKV->GetInt("syringe_effect", 1) != 0;
        fEffectTime = pKV->GetFloat("effect_time", 1.2f);
        g_szWeaponBlackList = pKV->GetString("weapon_blacklist", "weapon_molotov;weapon_hegrenade;");
        g_iMaxShotHP = pKV->GetInt("max_shot_hp", 50);
    }

    return true;
}

void OnStartupServer()
{
    g_pGameEntitySystem = g_pUtils->GetCGameEntitySystem();
    g_pEntitySystem = g_pUtils->GetCEntitySystem();
}

bool HealModule::Unload(char* error, size_t maxlen)
{
    delete g_pVIPCore;
    delete g_pUtils;
    return true;
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pUtils->GetCGameEntitySystem();
}

CBasePlayerWeapon* GetWeaponFromController(CCSPlayerController* pController) {
    if (!pController) return nullptr;
    CBasePlayerPawn* pPlayerPawn = pController->GetPawn();

    if (pPlayerPawn && pPlayerPawn->m_pWeaponServices) {
        CBasePlayerWeapon* hWeapon = pPlayerPawn->m_pWeaponServices->m_hActiveWeapon.Get();
        if (hWeapon) return hWeapon;
    }

    return nullptr;
}

void ApplySyringeEffect(CCSPlayerController* pVictimController)
{
    if (g_bSyringeEffectEnabled && pVictimController) {
        CCSPlayerPawn* pVictimPawn = pVictimController->GetPlayerPawn();
        if (pVictimPawn) {
            pVictimPawn->m_flHealthShotBoostExpirationTime.Get() = g_pUtils->GetCGlobalVars()->curtime + fEffectTime;
            g_pUtils->SetStateChanged(pVictimPawn, "CCSPlayerPawn", "m_flHealthShotBoostExpirationTime");
        }
    }
}

bool OnTakeDamage(int iVictimSlot, CTakeDamageInfo* pInfo)
{
    if (pInfo->m_bitsDamageType & DMG_FALL)
        return true;

    CCSPlayerPawn* pAttackerPawn = (CCSPlayerPawn*)pInfo->m_hAttacker.Get();
    if (!pAttackerPawn)
        return true;

    CBasePlayerController* pAttackerControllerBase = pAttackerPawn->m_hController.Get();
    if (!pAttackerControllerBase)
        return true;

    int iAttackerSlot = pAttackerControllerBase->GetEntityIndex().Get() - 1;
    if (iAttackerSlot < 0 || iAttackerSlot > 63)
        return true;

    if (iVictimSlot == iAttackerSlot)
        return true;

    CCSPlayerController* pVictimController = CCSPlayerController::FromSlot(iVictimSlot);
    if (!pVictimController)
        return true;

    CCSPlayerPawn* pVictimPawn = pVictimController->GetPlayerPawn();
    if (!pVictimPawn)
        return true;

    int   iVictimTeam   = pVictimPawn->m_iTeamNum();
    int   iAttackerTeam = pAttackerPawn->m_iTeamNum();
    float iDamage       = pInfo->m_flDamage;

    if (!g_bCanHeal[iAttackerSlot])
        return true;

    if (iVictimTeam == iAttackerTeam) {
        int iHealth    = pVictimPawn->m_iHealth;
        int iMaxHealth = pVictimPawn->m_iMaxHealth;

        CCSPlayerController* pAttackerController =
            static_cast<CCSPlayerController*>(pAttackerControllerBase);

        CBasePlayerWeapon* pWeapon = GetWeaponFromController(pAttackerController);
        if (pWeapon) {
            int DefIndex = pWeapon->m_AttributeManager().m_Item().m_iItemDefinitionIndex();
            std::string pszClassName;

            switch (DefIndex)
            {
                case 1:  pszClassName = "weapon_deagle";        break;
                case 23: pszClassName = "weapon_mp5sd";         break;
                case 60: pszClassName = "weapon_m4a1_silencer"; break;
                case 61: pszClassName = "weapon_usp_silencer";  break;
                case 63: pszClassName = "weapon_cz75a";         break;
                case 64: pszClassName = "weapon_revolver";      break;
                default: pszClassName = pWeapon->GetClassname(); break;
            }

            if (strstr(g_szWeaponBlackList, pszClassName.c_str()) == nullptr) {
                if (iHealth < iMaxHealth) {
                    int   iPercent       = g_pVIPCore->VIP_GetClientFeatureInt(iAttackerSlot, "heal_teammates");
                    float healPercentage = (float)iPercent / 100.0f;

                    int iHealAmount = static_cast<int>(iDamage * healPercentage);
                    if (iHealAmount > g_iMaxShotHP)
                        iHealAmount = g_iMaxShotHP;

                    iHealth += iHealAmount;
                    if (iHealth > iMaxHealth)
                        iHealth = iMaxHealth;

                    pVictimPawn->m_iHealth = iHealth;
                    g_pUtils->SetStateChanged(pVictimPawn, "CBaseEntity", "m_iHealth");

                    ApplySyringeEffect(pVictimController);
                }
            }
        }
    }

    return true;
}


void OnClientAuthorized(int iSlot, bool bIsVIP)
{
    if (!bIsVIP) return;
    g_bCanHeal[iSlot] = g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "heal_teammates");
}

bool OnToggle(int iSlot, const char* szFeature, VIP_ToggleState eOldStatus, VIP_ToggleState& eNewStatus)
{
    g_bCanHeal[iSlot] = (eNewStatus == ENABLED);
    return false;
}

void HealModule::AllPluginsLoaded()
{
    char error[64];
    int ret;

    g_pUtils = (IUtilsApi*)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);
    if (ret == META_IFACE_FAILED)
    {
        g_SMAPI->Format(error, sizeof(error), "Missing Utils system plugin");
        ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
        std::string sBuffer = "meta unload " + std::to_string(g_PLID);
        engine->ServerCommand(sBuffer.c_str());
        return;
    }

    g_pVIPCore = (IVIPApi*)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
    if (ret == META_IFACE_FAILED)
    {
        g_pUtils->ErrorLog("[%s] Missing VIP system plugin", GetLogTag());
        std::string sBuffer = "meta unload " + std::to_string(g_PLID);
        engine->ServerCommand(sBuffer.c_str());
        return;
    }

    g_pVIPCore->VIP_RegisterFeature("heal_teammates", VIP_STRING, TOGGLABLE, nullptr, OnToggle);
    g_pUtils->StartupServer(g_PLID, OnStartupServer);
    g_pVIPCore->VIP_OnClientLoaded(OnClientAuthorized);
    g_pUtils->HookOnTakeDamagePre(g_PLID, OnTakeDamage);
}

const char* HealModule::GetLicense()
{
    return "Public";
}

const char* HealModule::GetVersion()
{
    return "1.2.1";
}

const char* HealModule::GetDate()
{
    return __DATE__;
}

const char* HealModule::GetLogTag()
{
    return "[VIP-Heal]";
}

const char* HealModule::GetAuthor()
{
    return "ABKAM";
}

const char* HealModule::GetDescription()
{
    return "[VIP] Teammates Heal";
}

const char* HealModule::GetName()
{
    return "[VIP] Teammates Heal";
}

const char* HealModule::GetURL()
{
    return "https://discord.com/invite/g798xERK5Y";
}