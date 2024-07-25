#pragma once
#include <unordered_map>
#include <string>
#include <Windows.h>

class WindowsOSMessageLogger
{
public:
    WindowsOSMessageLogger();
    std::wstring operator()(DWORD msg, LPARAM lp, WPARAM wp) const; // Changed return type to std::wstring
private:
    std::unordered_map<DWORD, std::wstring> map; // Changed map to hold std::wstring
};
