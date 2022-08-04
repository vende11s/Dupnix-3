#include <vector>
#include <utility>
#include "execute.h"
#include "parse.h"
#include "telegram.h"
#include "constant.h"
#include "functions.h"

std::vector <std::pair<std::string, void(*)(std::string)>> functions{
	{"Ping", Ping}
};

namespace execute {
	void execute(parse::ParsedMessage PM) {
		if (PM.ID != ID)
			return;
		for (auto &i : functions) {
			if (i.first == PM.function) {
				i.second(PM.parameters);
				return;
			}
		}
		telegram::Send(ID + " function: " + PM.function + " not found.");
	}
}