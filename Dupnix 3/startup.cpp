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
	std::clog << "      _                   _      ____  " << std::endl
		<< "     | |                 (_)    |___ \\ " << std::endl
		<< "   __| |_   _ _ __  _ __  ___  __ __) |" << std::endl
		<< "  / _` | | | | '_ \\| '_ \\| \\ \\/ /|__ < " << std::endl
		<< " | (_| | |_| | |_) | | | | |>  < ___) |" << std::endl
		<< "  \\__,_|\\__,_| .__/|_| |_|_/_/\\_\\____/  " << std::endl
		<< "             | |  " << VERSION << std::endl
		<< "             |_|                       ";
}

void loadData() {
	json data;
	
	try {
		 data = tools::info::loadCfg();
	}
	catch (...) {
		data = DEFAULT_CONFIG;
	}

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

std::string argvToString(int argc, char** argv) {
	std::string output;

	for (int i = 1; i < argc; i++) {
		output += argv[i];
		output += " ";
	}
	return output;
}

void startup(int argc, char **argv) {
	// cd to the folder dupnix is inside
	tools::CdToDefault();

	// if config file doesn't exists then put whatever is in argv to this file
	if (!tools::info::filExists(CONFIG_FILENAME)) {
		std::ofstream f(CONFIG_FILENAME);
		f << argvToString(argc, argv);
		f.close();
	}

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

	INSTANCE_ID = tools::randomString(8);
	std::clog << "\ninstance ID: " << INSTANCE_ID << std::endl;

	telegram::SendText(ID + " is running");
	std::clog << "ID: " + ID << std::endl;
}
