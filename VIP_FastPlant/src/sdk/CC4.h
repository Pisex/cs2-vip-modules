#pragma once
#include <entity2/entityidentity.h>
#include "CBaseAnimGraph.h"
#include "schemasystem.h"

struct GameTime_t
{
public:
	float m_Value;
};

class CC4
{
public:
	SCHEMA_FIELD(GameTime_t, CC4, m_fArmedTime);
};