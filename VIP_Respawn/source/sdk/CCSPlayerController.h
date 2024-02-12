#pragma once
#include "CCSPlayerController_InGameMoneyServices.h"
#include "CBasePlayerController.h"
#include "CCSPlayerPawn.h"
#include "ehandle.h"
#include "virtual.h"
#include "schemasystem.h"

class CCSPlayerController : public CBasePlayerController
{
public:
	SCHEMA_FIELD(CUtlSymbolLarge, CCSPlayerController, m_szClan);
	SCHEMA_FIELD(CHandle<CCSPlayerPawn>, CCSPlayerController, m_hPlayerPawn);
	SCHEMA_FIELD(CCSPlayerController_InGameMoneyServices*, CCSPlayerController, m_pInGameMoneyServices);

    auto ForceRespawn() {
		return CALL_VIRTUAL(void, WIN_LINUX(245, 247), this);
	}
};