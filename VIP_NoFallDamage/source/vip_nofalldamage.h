#ifndef _INCLUDE_VIP_NOFALLDAMAGE_MODULE_H_
#define _INCLUDE_VIP_NOFALLDAMAGE_MODULE_H_

#include <ISmmPlugin.h>
#include <igameevents.h>
#include <iplayerinfo.h>
#include "utlvector.h"
#include "ehandle.h"
#include <sh_vector.h>
#include <entity2/entitysystem.h>
#include "CCSPlayerController.h"
#include "ctakedamageinfo.h"
#include "iserver.h"
#include "include/vip.h"
#include "include/menus.h"

class NoFallDamageModule : public ISmmPlugin, public IMetamodListener
{
public:
    bool Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late) override;
    bool Unload(char* error, size_t maxlen) override;
    void AllPluginsLoaded() override;

public:
    const char* GetAuthor() override;
    const char* GetName() override;
    const char* GetDescription() override;
    const char* GetURL() override;
    const char* GetLicense() override;
    const char* GetVersion() override;
    const char* GetDate() override;
    const char* GetLogTag() override;

};

extern NoFallDamageModule g_NoFallDamageModule;

PLUGIN_GLOBALVARS();

#endif 
