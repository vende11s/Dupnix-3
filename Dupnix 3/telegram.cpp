#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include "constant.h"
#include "telegram.h"

using json = nlohmann::json;

//https://stackoverflow.com/questions/154536/encode-decode-urls-in-c
std::string url_encode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char)c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}

namespace telegram {
    void Send(const std::string& send) {
        std::string to_send = send;
        while (!to_send.empty()) {
            cpr::Response r = cpr::Get(cpr::Url{ "https://api.telegram.org/bot" + BOT_API + "/sendMessage?chat_id=" + CHAT_ID + "&text=" + url_encode(to_send.substr(0,TELEGRAM_MAX)) });
            to_send.erase(0, TELEGRAM_MAX - 1);
        }
    }

    json bad_json() {
        json bad;
        bad["message_id"] = 69;
        bad["date"] = -1;
        return bad;
    }

    json GetLastMessage() {

        auto response = cpr::Get(cpr::Url{ "https://api.telegram.org/bot" + BOT_API + "/getUpdates?last=1&offset=-1" });
        std::string& raw_response = response.text;

        if (!raw_response.empty())
            raw_response = raw_response.substr(21, raw_response.size() - 23);

        if (raw_response.empty() || raw_response == "\n")
            return bad_json();


        static bool error(false);
        json LastMessage;

        try {
            LastMessage = json::parse(raw_response);
        }
        catch (json::parse_error& e)
        {
            std::cerr << "\n\n" << "message: " << e.what() << '\n'
                << "exception id: " << e.id << '\n'
                << "byte position of error: " << e.byte << std::endl;
        #ifndef NDEBUG
            std::cerr << "raw message: " << raw_response << std::endl;
        #endif
            if (!error) 
                Send(ID + " something went wrong with parsing to json");
            error = true;

            return bad_json();
        }
        error = false;
        return LastMessage["channel_post"];
    }
}

