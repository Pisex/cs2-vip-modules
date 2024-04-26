#pragma once
#include "CCSPlayerPawnBase.h"
#include "schemasystem.h"

class CCSPlayerPawn : public CCSPlayerPawnBase
{
public:
	SCHEMA_FIELD(int32_t, CCSPlayerPawn, m_ArmorValue);
};