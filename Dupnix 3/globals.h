#pragma once
#include <string>
#include <set>

#include <nlohmann/json.hpp>

extern  int REFRESH;  // in seconds
extern  int TELEGRAM_MAX;
extern  bool AUTOSTART;
extern  bool HIDE_TERMINAL;
extern  bool MAIN_INSTANCE_MODE;
extern  std::string ID;
extern  std::string BOT_API;
extern  std::string CHAT_ID;
extern  std::string VERSION;
extern  std::string PATH;  // you can use %sysdisk% and %username%
extern  std::string CONFIG_FILENAME;
extern  nlohmann::json DEFAULT_CONFIG;
extern  std::string INSTANCE_ID;
extern  std::set<char> DISK_LIST;
extern	std::string UPDATE_LINK;
