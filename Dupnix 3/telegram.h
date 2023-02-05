#pragma once
#include <string>

#include <nlohmann/json.hpp>

namespace telegram {
	void SendText(const std::string& send);
	void SendPhoto(const std::string& photo_path);
	bool SendFile(const std::string& file_path);

	nlohmann::json badJson();
	nlohmann::json GetLastMessage();
}
