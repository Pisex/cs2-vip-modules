#pragma once
#include "CBaseCombatCharacter.h"
#include "CPlayerPawnComponent.h"
#include "schemasystem.h"

class CInButtonState
{
public:
    SCHEMA_FIELD(uint64_t[3], CInButtonState, m_pButtonStates);
};

class CPlayer_MovementServices : public CPlayerPawnComponent
{
public:
    SCHEMA_FIELD(CInButtonState, CPlayer_MovementServices, m_nButtons);
};

class CBasePlayerPawn : public CBaseCombatCharacter
{
public:
    SCHEMA_FIELD(CPlayer_MovementServices*, CBasePlayerPawn, m_pMovementServices);
};