#pragma once
#include "CBaseCombatCharacter.h"
#include "CPlayer_ItemServices.h"
#include "CPlayerPawnComponent.h"
#include "schemasystem.h"
#include "ehandle.h"

class CPlayer_WeaponServices : public CPlayerPawnComponent
{
public:
    SCHEMA_FIELD(CHandle<int>, CPlayer_WeaponServices, m_hActiveWeapon);
    SCHEMA_FIELD(uint16_t[32], CPlayer_WeaponServices, m_iAmmo);
};

class CBasePlayerPawn : public CBaseCombatCharacter
{
public:
    SCHEMA_FIELD(CPlayer_ItemServices*, CBasePlayerPawn, m_pItemServices);
    SCHEMA_FIELD(CPlayer_WeaponServices*, CBasePlayerPawn, m_pWeaponServices);
};