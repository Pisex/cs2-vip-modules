#include <stdio.h>
#include "vip_time.h"
#include <fstream>
#include "entitykeyvalues.h"
#include <sstream>

vip_time g_vip_time;

IUtilsApi* g_pUtils;
IMenusApi* g_pMenus;
IVIPApi* g_pVIPCore;

float g_flUniversalTime;
float g_flLastTickedTime;
bool g_bHasTicked;

IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;

bool g_bGive;
bool g_bGived[64];
char g_szGroup[64];
char g_sStartTime[64];
char g_sEndTime[64];

PLUGIN_EXPOSE(vip_time, g_vip_time);

SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);

CGameEntitySystem* GameEntitySystem()
{
	return g_pUtils->GetCGameEntitySystem();
};

void StartupServer()
{
	g_bGive = false;
	g_bHasTicked = false;
	g_pGameEntitySystem = GameEntitySystem();
	g_pEntitySystem = g_pUtils->GetCEntitySystem();
}

constexpr int HOURS = 0;
constexpr int MINUTES = 1;

void FormatTime(char* buffer, size_t bufferSize, const char* format) {
    time_t t = std::time(0);
    struct tm *tm = localtime(&t); 
    strftime(buffer, bufferSize, format, tm);
}

void GetIntDate(const std::string& szTime, int iDate[2]);
bool IsTimeInPeriod(const int iTime[2], const int iStartTime[2], const int iEndTime[2]);
void RemoveVipFromClients();
void GiveVipForClients();
void GiveVipToClient(int iSlot);

bool vip_time::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetServerFactory, g_pSource2GameClients, IServerGameClients, SOURCE2GAMECLIENTS_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetServerFactory, g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	ConVar_Register(FCVAR_RELEASE | FCVAR_CLIENT_CAN_EXECUTE | FCVAR_GAMEDLL);

	SH_ADD_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_time::GameFrame), true);

	{
		KeyValues* hKv = new KeyValues("VIP");
		const char *pszPath = "addons/configs/vip/vip_time.ini";

		if (!hKv->LoadFromFile(g_pFullFileSystem, pszPath))
		{
			Warning("Failed to load %s\n", pszPath);
			return false;
		}

		g_SMAPI->Format(g_szGroup, sizeof(g_szGroup), "%s", hKv->GetString("group"));
		g_SMAPI->Format(g_sStartTime, sizeof(g_sStartTime), "%s", hKv->GetString("start_time"));
		g_SMAPI->Format(g_sEndTime, sizeof(g_sEndTime), "%s", hKv->GetString("end_time"));
		delete hKv;
	}

	new CTimer(60.0f, [](){
		char szTime[64];
		int iCurrentTime[2], iStartTime[2], iEndTime[2];
		int iMidnight[2] = {24, 0}; // Время полуночи
		int iStartOfDay[2] = {0, 0}; // Начало дня

		// Получение и преобразование текущего времени
		FormatTime(szTime, sizeof(szTime), "%H:%M");
		GetIntDate(szTime, iCurrentTime);
		GetIntDate(g_sStartTime, iStartTime);
		GetIntDate(g_sEndTime, iEndTime);

		bool bGive = g_bGive;

		// Проверка временного периода и обновление статуса VIP
		if(iEndTime[HOURS] < iStartTime[HOURS]) {
			g_bGive = IsTimeInPeriod(iCurrentTime, iStartTime, iMidnight) ||
					IsTimeInPeriod(iCurrentTime, iStartOfDay, iEndTime);
		} else {
			g_bGive = IsTimeInPeriod(iCurrentTime, iStartTime, iEndTime);
		}

		// Обновление VIP статуса клиентов
		if (!g_bGive && bGive) {
			RemoveVipFromClients();
		} else if (g_bGive && !bGive) {
			GiveVipForClients();
		}
		return 60.0f;
	});

	g_SMAPI->AddListener( this, this );
	return true;
}

void RemoveVipFromClients()
{
	for (int i = 0; i < 64; i++)
	{
		if (g_bGived[i] && g_pVIPCore->VIP_IsClientVIP(i))
		{
			g_bGived[i] = false;
			g_pVIPCore->VIP_RemoveClientVIP(i, false, false);
		}
	}
}

void GiveVipToClient(int iSlot)
{
	g_pVIPCore->VIP_GiveClientVIP(iSlot, 0, g_szGroup, false);
	g_bGived[iSlot] = true;
}

void GiveVipForClients()
{
	for (int i = 0; i < 64; i++)
	{
		if (!g_bGived[i] && !g_pVIPCore->VIP_IsClientVIP(i))
		{
			GiveVipToClient(i);
		}
	}
}

void GetIntDate(const std::string& szTime, int iDate[2]) {
    size_t colonPos = szTime.find(':');
    if (colonPos != std::string::npos) {
        iDate[HOURS] = std::stoi(szTime.substr(0, colonPos));
        iDate[MINUTES] = std::stoi(szTime.substr(colonPos + 1));
    }
}

bool IsTimeInPeriod(const int iTime[2], const int iStartTime[2], const int iEndTime[2])
{
	if(iTime[HOURS] == iStartTime[HOURS])
	{
		if(iTime[HOURS] == iEndTime[HOURS])
		{
			return (iTime[MINUTES] >= iStartTime[MINUTES] && iTime[MINUTES] < iEndTime[MINUTES]);
		}

		return (iTime[MINUTES] >= iStartTime[MINUTES]);
	}

	if(iTime[HOURS] > iStartTime[HOURS])
	{
		if(iTime[HOURS] < iEndTime[HOURS])
		{
			return true;
		}

		if(iTime[HOURS] == iEndTime[HOURS])
		{
			return (iTime[MINUTES] < iEndTime[MINUTES]);
		}
	}

	return false;
}

bool vip_time::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, g_pSource2Server, SH_MEMBER(this, &vip_time::GameFrame), true);
	return true;
}

void vip_time::GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	if (simulating && g_bHasTicked)
	{
		g_flUniversalTime += g_pUtils->GetCGlobalVars()->curtime - g_flLastTickedTime;
	}

	g_flLastTickedTime = g_pUtils->GetCGlobalVars()->curtime;
	g_bHasTicked = true;

	for (int i = g_timers.Tail(); i != g_timers.InvalidIndex();)
	{
		auto timer = g_timers[i];

		int prevIndex = i;
		i = g_timers.Previous(i);

		if (timer->m_flLastExecute == -1)
			timer->m_flLastExecute = g_flUniversalTime;

		// Timer execute 
		if (timer->m_flLastExecute + timer->m_flInterval <= g_flUniversalTime)
		{
			if (!timer->Execute())
			{
				delete timer;
				g_timers.Remove(prevIndex);
			}
			else
			{
				timer->m_flLastExecute = g_flUniversalTime;
			}
		}
	}
}

void OnClientLoaded(int iSlot, bool bIsVIP)
{
	if(g_bGive && !bIsVIP)
	{
		GiveVipToClient(iSlot);
	}
}

void vip_time::AllPluginsLoaded()
{
	char error[64];
	int ret;
	g_pVIPCore = (IVIPApi*)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		char error[64];
		V_strncpy(error, "Failed to lookup vip core. Aborting", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pUtils = (IUtilsApi*)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);
	if (ret == META_IFACE_FAILED)
	{
		V_strncpy(error, "Failed to lookup menus core. Aborting", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	g_pUtils->StartupServer(g_PLID, StartupServer);
	g_pVIPCore->VIP_OnClientLoaded(OnClientLoaded);
}

const char *vip_time::GetLicense()
{
	return "Public";
}

const char *vip_time::GetVersion()
{
	return "1.0";
}

const char *vip_time::GetDate()
{
	return __DATE__;
}

const char *vip_time::GetLogTag()
{
	return "[Time VIP]";
}

const char *vip_time::GetAuthor()
{
	return "Pisex";
}

const char *vip_time::GetDescription()
{
	return "";
}

const char *vip_time::GetName()
{
	return "[VIP] Time VIP";
}

const char *vip_time::GetURL()
{
	return "https://discord.com/invite/g798xERK5Y";
}
