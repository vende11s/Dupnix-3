#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace telegram {
	void SendText(const std::string& send);
	void SendPhoto(const std::string& PhotoPath);
	bool SendFile(const std::string& FilePath);

	nlohmann::json bad_json();
	nlohmann::json GetLastMessage();
}
