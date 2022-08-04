#include <iostream>
#include <string>
#include <shlwapi.h>
#include <Windows.h>
#include <memory>
#include <array>
#include <exception>
#include <cpr/cpr.h>
#include "tools.h"

#pragma comment(lib, "Shlwapi.lib")

BOOL IsProcessElevated();

namespace tools {
    char GetSysDiskLetter() {
        char letter[256 + 1];
        GetSystemDirectory(letter, sizeof(letter));
        return letter[0];
    }

    std::string get_username() {
        std::string username = cmd_output("echo %username%");
        return username.substr(0, username.size() - 1);
    }

    std::string cmd_output(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
        if (!pipe) {
            std::cerr<<"popen() failed!\n";
            return "";
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    bool filexists(const std::string& name) {
        struct stat buffer;
        return (stat(name.c_str(), &buffer) == 0);
    }


    std::string get_exe() {
        //https://stackoverflow.com/questions/10814934/how-can-program-get-executable-name-of-itself
        TCHAR buffer[MAX_PATH] = { 0 };
        TCHAR* out;
        DWORD bufSize = sizeof(buffer) / sizeof(*buffer);
        GetModuleFileName(NULL, buffer, bufSize);
        out = PathFindFileName(buffer);
        return out;
    }

    std::string hostname() {
        std::string buffer = cmd_output("hostname");
        buffer[buffer.size() - 1] = ' ';
        return buffer;
    }

    std::string public_ip() {
        auto response = cpr::Get(cpr::Url{ "https://myexternalip.com/raw" });
        return response.text;
    }

    std::string admin_rights() {
        return std::to_string(IsProcessElevated());
    }

    std::string Cursor_Position() {
        POINT p;
        GetCursorPos(&p);
        return std::to_string(p.x) + "," + std::to_string(p.y);
    }

    std::string get_path() {
        //https://stackoverflow.com/questions/10814934/how-can-program-get-executable-name-of-itself
        TCHAR buffer[MAX_PATH] = { 0 };
        DWORD bufSize = sizeof(buffer) / sizeof(*buffer);
        GetModuleFileName(NULL, buffer, bufSize);
        std::string buff = buffer;
        return buff.substr(0, buff.size() - get_exe().size());
    }

    std::string random_string(int lenght) {
        srand((unsigned int)time(0));
        std::string random = "";
        while (lenght--) {
            char ran = rand() % 25 + 97;
            random += ran;
        }
        return random;
    }

    bool is_path(const std::string& path) {
        struct stat s;
        if (stat(path.c_str(), &s) == 0) 
            if (s.st_mode & S_IFDIR)
                return true;
      
        return false;
    }

    bool remove(const std::string& to_remove) {
        if (is_path(to_remove)) {
            return !system(std::string("rmdir /s /q " + to_remove).c_str());
        }

        return !system(std::string("del /f /q " + to_remove).c_str());
    }
}

BOOL IsProcessElevated()
{
    BOOL fIsElevated = FALSE;
    HANDLE hToken = NULL;
    TOKEN_ELEVATION elevation;
    DWORD dwSize;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        printf("\n Failed to get Process Token :%d.", GetLastError());
        goto Cleanup;  // if Failed, we treat as False
    }


    if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
    {
        printf("\nFailed to get Token Information :%d.", GetLastError());
        goto Cleanup;// if Failed, we treat as False
    }

    fIsElevated = elevation.TokenIsElevated;

Cleanup:
    if (hToken)
    {
        CloseHandle(hToken);
        hToken = NULL;
    }
    return fIsElevated;
}