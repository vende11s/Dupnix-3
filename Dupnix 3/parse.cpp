#include "parse.h"

#include <string>

namespace parse {
	ParsedMessage Parse(const std::string& raw_text) {
		ParsedMessage output;
		for (int i = 0, it = 0; i < raw_text.size(); i++) {  // parsing message
			if (raw_text[i] == ' ') {
				it++;
				if (it <= 2)
					continue;
			}

			if (it == 0)
				output.ID += raw_text[i];
			if (it == 1)
				output.function += raw_text[i];
			if (it > 1)
				output.parameters += raw_text[i];
		}
		return output;
	}
}  // namespace parse
