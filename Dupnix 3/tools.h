#pragma once
#ifdef _DEBUG
#define PRINT_DEBUG_VALUE(v) std::clog << "Debug value " << #v << " " << v << std::endl;
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
	std::string randomString(int lenght);
	int ChangeVolume(double nVolume = -1, bool bScalar = 0);
	void changeCfg(nlohmann::json change);
	void press_key(char a, bool is_bigone = 0);
	bool DownloadFile(std::string link, std::string path);
	std::string toLowerCase(std::string s);
	void pressSpecialKey(BYTE key, bool up);
	bool remove(const std::string& to_remove);

	// get info
	namespace info {
		nlohmann::json loadCfg();
		char getSysDiskLetter();
		std::string getUsername();
		std::string getExeName();
		std::string getHostname();
		std::string getPublicIp();
		bool checkAdminRights();  // to check if dupnix has admin rights
		std::string getCursorPos(); 
		std::string getDupnixPath(); 
		std::string getUptime();
		std::string DiskList(); 
		std::vector<std::pair<std::string, std::string>> getLocalIp();
		bool isDir(const std::string& path);
		std::string cmdOutput(const char* cmd); 
		bool filExists(const std::string& name);
	}  //namespace info

	// for screenshot function
	cv::Mat captureScreenMat(HWND hwnd);
}  // namespace tools
BITMAPINFOHEADER createBitmapHeader(int width, int height);