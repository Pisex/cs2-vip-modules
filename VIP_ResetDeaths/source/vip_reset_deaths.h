#ifndef _INCLUDE_VIP_RESET_DEATHS_H_
#define _INCLUDE_VIP_RESET_DEATHS_H_

#include <ISmmPlugin.h>
#include <igameevents.h>
#include <iplayerinfo.h>
#include "utlvector.h"
#include "ehandle.h"
#include <sh_vector.h>
#include <entity2/entitysystem.h>
#include "CCSPlayerController.h"
#include "iserver.h"
#include "include/vip.h"
#include "include/menus.h"
#include <ctime>

class VIPResetDeaths : public ISmmPlugin, public IMetamodListener
{
public:
	bool Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late);
	bool Unload(char* error, size_t maxlen);
	void AllPluginsLoaded();
public:
	const char* GetAuthor();
	const char* GetName();
	const char* GetDescription();
	const char* GetURL();
	const char* GetLicense();
	const char* GetVersion();
	const char* GetDate();
	const char* GetLogTag();
};

extern VIPResetDeaths g_VIPResetDeaths;

PLUGIN_GLOBALVARS();

#endif 
