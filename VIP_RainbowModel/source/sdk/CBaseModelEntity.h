#pragma once
#include "CBaseEntity.h"
#include "schemasystem.h"

class CBaseModelEntity : public SC_CBaseEntity
{
public:
    SCHEMA_FIELD(Color, CBaseModelEntity, m_clrRender);
};