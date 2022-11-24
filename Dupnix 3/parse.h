#pragma once

#include <string>

namespace parse {
	struct ParsedMessage {
		std::string ID;
		std::string function;
		std::string parameters;
	};

	ParsedMessage Parse(const std::string& raw_text);
}
