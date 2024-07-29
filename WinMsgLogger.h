#pragma once
#include <string>
#include <Windows.h>
#include <unordered_map>

class WinMsgLogger
{
public:
    WinMsgLogger();
    std::wstring operator()(DWORD msg, LPARAM lp, WPARAM wp) const; // Changed return type to std::wstring
private:
    std::unordered_map<DWORD, std::wstring> map; // Changed map to hold std::wstring
};
