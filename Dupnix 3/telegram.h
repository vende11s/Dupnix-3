#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace telegram {
	void Send(const std::string& send);
	nlohmann::json bad_json();
	nlohmann::json GetLastMessage();
}