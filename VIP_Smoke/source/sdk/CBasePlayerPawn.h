#pragma once
#include "CBaseCombatCharacter.h"
#include "schemasystem.h"

class CBasePlayerPawn : public CBaseCombatCharacter
{
public:
	SCHEMA_FIELD(CHandle<CBasePlayerController>, CBasePlayerPawn, m_hController);
};