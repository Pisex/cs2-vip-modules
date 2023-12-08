#pragma once
#include "CBasePlayerController.h"
#include "ehandle.h"
#include "schemasystem.h"

class CCSPlayerController : public CBasePlayerController
{
public:
	SCHEMA_FIELD(CUtlSymbolLarge, CCSPlayerController, m_szClan);
};