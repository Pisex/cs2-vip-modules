#pragma once
#include "CBasePlayerPawn.h"
#include "schemasystem.h"

class CCSPlayerPawnBase : public CBasePlayerPawn
{
public:
    SCHEMA_FIELD(float, CCSPlayerPawnBase, m_flVelocityModifier);
};