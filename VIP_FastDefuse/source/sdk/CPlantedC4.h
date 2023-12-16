#pragma once
#include <entity2/entityidentity.h>
#include "CBaseAnimGraph.h"
#include "schemasystem.h"

struct GameTime_t
{
public:
	float m_Value;
};

class CPlantedC4 : public CBaseAnimGraph
{
public:
	SCHEMA_FIELD(GameTime_t, CPlantedC4, m_flC4Blow);
	SCHEMA_FIELD(GameTime_t, CPlantedC4, m_flDefuseCountDown);
};