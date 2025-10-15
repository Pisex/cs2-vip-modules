#pragma once

#include <playerslot.h>
#include "igameevents.h"
#include <functional>
#include <string>

#define VIP_INTERFACE "IVIPApi"

enum VIP_ValueType
{
	VIP_NULL	= 0,	// Нет данных
	VIP_INT,				// Целочисленный тип
	VIP_FLOAT,				// Числа с плавающей точкой (Дробные)
	VIP_BOOL,			// Логический тип (1/0)
	VIP_STRING				// Строчный тип
};

enum VIP_ToggleState
{
	DISABLED = 0,		// Выключено
	ENABLED,			// Включено
	NO_ACCESS			// Нет доступа
};

enum VIP_FeatureType
{
	TOGGLABLE = 0,		// Вкл/Выкл в меню
	SELECTABLE,			// Только нажатие
	HIDE				// Скрытый
};


// (для типа TOGGLABLE)
typedef std::function<bool(int iSlot, const char* szFeature, VIP_ToggleState eOldStatus, VIP_ToggleState& eNewStatus)> ItemTogglableCallback;
// (для типа SELECTABLE)
typedef std::function<bool(int iSlot, const char* szFeature)> ItemSelectableCallback;

// Визуал
typedef std::function<std::string(int iSlot, const char* szFeature)> ItemDisplayCallback;


typedef std::function<void()> ReadyCallbackFunc;
typedef std::function<void(int iSlot, int iTeam, bool bIsVIP)> SpawnCallbackFunc;
typedef std::function<void(int iSlot, bool bIsVIP)> ClientLoadedOrDisconnectCallbackFunc;
typedef std::function<void(int iSlot)> VIPAddCallbackFunc;
typedef std::function<void(int iSlot, int iReason)> VIPRemoveCallbackFunc;

class IVIPApi
{
public:
	virtual bool VIP_IsVIPLoaded() = 0;
	virtual bool VIP_IsClientVIP(int iSlot) = 0;
	virtual int VIP_GetClientAccessTime(int iSlot) = 0;
	virtual bool VIP_SetClientAccessTime(int iSlot, int iTime, bool bInDB = true) = 0;
	virtual bool VIP_SetClientVIPGroup(int iSlot, const char* szGroup, bool bInDB = true) = 0;
	virtual const char *VIP_GetClientVIPGroup(int iSlot) = 0;

	virtual bool VIP_GiveClientVIP(int iSlot, int iTime, const char* szGroup, bool bAddToDB = 1) = 0;
	virtual bool VIP_RemoveClientVIP(int iSlot, bool bNotify, bool bInDB = 1) = 0;

	virtual int VIP_GetClientFeatureInt(int iSlot, const char* szFeature) = 0;
	virtual bool VIP_GetClientFeatureBool(int iSlot, const char* szFeature) = 0;
	virtual float VIP_GetClientFeatureFloat(int iSlot, const char* szFeature) = 0;
	virtual const char *VIP_GetClientFeatureString(int iSlot, const char* szFeature) = 0;

	virtual CGameEntitySystem* VIP_GetEntitySystem() = 0;
    virtual int VIP_GetTotalRounds() = 0;

	virtual bool VIP_IsValidVIPGroup(const char* szGroup) = 0;

	virtual bool VIP_PistolRound() = 0;
	virtual bool VIP_WarmupPeriod() = 0;

	virtual bool VIP_SetClientCookie(int iSlot, const char* sCookieName, const char* sData) = 0;
	virtual const char *VIP_GetTranslate(const char* phrase) = 0;
	virtual const char *VIP_GetClientCookie(int iSlot, const char* sCookieName) = 0;

	virtual void VIP_PrintToCenter(int iSlot, const char* msg, ...) = 0;

	//Forwards
    virtual void VIP_OnVIPLoaded(ReadyCallbackFunc callback) = 0;
	virtual void VIP_OnClientLoaded(ClientLoadedOrDisconnectCallbackFunc callback) = 0;
	virtual void VIP_OnClientDisconnect(ClientLoadedOrDisconnectCallbackFunc callback) = 0;
	virtual void VIP_OnPlayerSpawn(SpawnCallbackFunc callback) = 0;
	virtual void VIP_OnVIPClientRemoved(VIPRemoveCallbackFunc callback) = 0;
	virtual void VIP_OnVIPClientAdded(VIPAddCallbackFunc callback) = 0;

	
    virtual void VIP_RegisterFeature(const char*			    szFeature,
								VIP_ValueType			eValType				= VIP_NULL,
								VIP_FeatureType			eType					= TOGGLABLE,
								ItemSelectableCallback	Item_select_callback	= nullptr,
								ItemTogglableCallback	Item_togglable_callback	= nullptr,
								ItemDisplayCallback		Item_display_callback	= nullptr) = 0;
};