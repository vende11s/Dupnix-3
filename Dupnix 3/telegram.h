#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace telegram {
	void Send(const std::string& send);
	void SendPhoto(const std::string& PhotoPath);
	void SendFile(const std::string& FilePath);

	nlohmann::json bad_json();
	nlohmann::json GetLastMessage();
}