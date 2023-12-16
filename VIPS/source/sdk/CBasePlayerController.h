#pragma once
#include "CBaseEntity.h"
#include "CBasePlayerPawn.h"
#include "ehandle.h"
#include "schemasystem.h"

class CBasePlayerController : public SC_CBaseEntity
{
public:
	SCHEMA_FIELD(char[128], CBasePlayerController, m_iszPlayerName);
};