#ifndef _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
#define _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_

#include <ISmmPlugin.h>
#include <igameevents.h>
#include <iplayerinfo.h>
#include "utlvector.h"
#include "ehandle.h"
#include <sh_vector.h>
#include <entity2/entitysystem.h>
#include "utils.hpp"
#include "CCSPlayerController.h"
#include "CC4.h"
#include "CGameRules.h"
#include "iserver.h"
#include "include/vip.h"
#include "include/menus.h"
#include <ctime>
#include <deque>
#include <functional>
#include <cmath>

class vip_fp : public ISmmPlugin, public IMetamodListener
{
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t maxlen);
	void AllPluginsLoaded();
	void NextFrame(std::function<void()> fn);
public:
	const char *GetAuthor();
	const char *GetName();
	const char *GetDescription();
	const char *GetURL();
	const char *GetLicense();
	const char *GetVersion();
	const char *GetDate();
	const char *GetLogTag();
private:
	void GameFrame(bool simulating, bool bFirstTick, bool bLastTick);
	std::deque<std::function<void()>> m_nextFrame;
};

extern vip_fp g_vip_fp;

PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
