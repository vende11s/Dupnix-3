#pragma once
#include <string>

namespace tools {
	//do something
	std::string cmd_output(const char* cmd);
	bool filexists(const std::string& name);
	std::string random_string(int lenght);
	
	//get info
	char GetSysDiskLetter();
	std::string get_username();
	std::string get_exe();
	std::string hostname();
	std::string public_ip();
	std::string admin_rights();
	std::string Cursor_Position();
	std::string get_path();
	bool is_path(const std::string& path);
	bool remove(const std::string& to_remove);
}