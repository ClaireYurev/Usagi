#include "WinMsgLogger.h"
#include "WinMsgDefinitions.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>

WinMsgLogger::WinMsgLogger() {
    for (const auto& def : messageDefinitions) {
        map.emplace(def.msg, def.name);
    }
}

std::wstring WinMsgLogger::operator()(DWORD msg, LPARAM lp, WPARAM wp) const {
    constexpr int firstColWidth = 25;
    const auto i = map.find(msg);

    std::wostringstream oss; // Use wide string stream
    if (i != map.end()) {
        oss << std::left << std::setw(firstColWidth) << i->second << std::right;
    }
    else {
        std::wostringstream padss;
        padss << L"Unrecognized message: 0x" << std::hex << msg;
        oss << std::left << std::setw(firstColWidth) << padss.str() << std::right;
    }
    oss << L"   LP: 0x" << std::hex << std::setfill(L'0') << std::setw(8) << lp;
    oss << L"   WP: 0x" << std::hex << std::setfill(L'0') << std::setw(8) << wp << std::endl;

    return oss.str();
}
