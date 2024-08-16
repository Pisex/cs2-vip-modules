#pragma once

#define FH_INTERFACE "IFortniteHitsApi"
class IFortniteHitsApi
{
public:
	virtual void GiveClientAccess(int iSlot) = 0;
	virtual void TakeClientAccess(int iSlot) = 0;
};