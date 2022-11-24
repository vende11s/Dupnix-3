#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace telegram {
	void SendText(const std::string& send);
	void SendPhoto(const std::string& photo_path);
	bool SendFile(const std::string& file_path);

	nlohmann::json badJson();
	nlohmann::json GetLastMessage();
}
