#include <string>
#include <set>

#include "globals.h"
#include "tools.h"
#include "diskchanges.h"

int REFRESH = 2;  // in seconds
int TELEGRAM_MAX = 4096;
bool AUTOSTART = false;
bool HIDE_TERMINAL = false;
std::string ID = "db";
std::string BOT_API = "";  // https://api.telegram.org/bot1799119274:AAFMecQgld8WXiPUu8_dHKWa_-qJFOkC664/getUpdates
std::string CHAT_ID = "";
std::string PATH = "";  // you can use $sysdisk$ and $username$
std::string VERSION = "v1.3.2";
std::string CONFIG_FILENAME = "dupnix-config.json";
nlohmann::json DEFAULT_CONFIG = "{\"autostart\": true,\"hide_terminal\" : true,\"refresh_rate\" : 2,\"id\" : \"2137\",\"path\" : \"$sysdisk$/users/public\"}";
std::string INSTANCE_ID = "";
std::set<char> DISK_LIST = diskchanges::GetDiskList();
std::string UPDATE_LINK = "https://github.com/vende11s/Dupnix-3/releases/latest/download/Dupnix3.exe";
