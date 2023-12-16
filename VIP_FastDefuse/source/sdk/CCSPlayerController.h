#pragma once
#include "CBasePlayerController.h"
#include "CCSPlayerPawn.h"
#include "CCSPlayerController_InGameMoneyServices.h"
#include "ehandle.h"
#include "schemasystem.h"

class CCSPlayerController : public CBasePlayerController
{
public:
	SCHEMA_FIELD(CHandle<CCSPlayerPawn>, CCSPlayerController, m_hPlayerPawn);
	SCHEMA_FIELD(CCSPlayerController_InGameMoneyServices*, CCSPlayerController, m_pInGameMoneyServices);
};