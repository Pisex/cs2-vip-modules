#pragma once
#include "CBaseEntity.h"
#include "CBasePlayerPawn.h"
#include "ehandle.h"
#include "schemasystem.h"

class CBasePlayerController : public SC_CBaseEntity
{
public:
    SCHEMA_FIELD(uint32_t, CBasePlayerController, m_iDesiredFOV);
    SCHEMA_FIELD(uint64_t, CBasePlayerController, m_steamID);
    SCHEMA_FIELD(CHandle<CBasePlayerPawn>, CBasePlayerController, m_hPawn);
};