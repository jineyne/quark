#include "Logger.h"

#include <chrono>

#include "LogDefines.h"

DEFINE_LOG_CATEGORY(LogTemp)

#define STARTING_BUFFER_SIZE 512

namespace OutputDeviceColor {
    const TCHAR * const COLOR_NORMAL = TEXT("\x1B[0m");
    const TCHAR * const COLOR_RED = TEXT("\x1B[31m");
    const TCHAR * const COLOR_GREEN = TEXT("\x1B[32m");
    const TCHAR * const COLOR_YELLOW = TEXT("\x1B[33m");
    const TCHAR * const COLOR_BLUE = TEXT("\x1B[34m");
    const TCHAR * const COLOR_MAG = TEXT("\x1B[35m");
    const TCHAR * const COLOR_CYAN = TEXT("\x1B[36m");
    const TCHAR * const COLOR_WHITE = TEXT("\x1B[37m");

    const TCHAR * const BACKGROUND_NORMAL = TEXT("\x1b[40m");
    const TCHAR * const BACKGROUND_RED = TEXT("\x1b[41m");
    const TCHAR * const BOLD = TEXT("\x1b[1m");
    const TCHAR * const BOLD_OFF = TEXT("\x1b[22m");

}

std::string TimeStamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%F %T");
    return ss.str();
}

Logger::Logger() {
}

void Logger::log(const String &categoryName, ELogLevel level, const TCHAR *message) {
    switch (level) {
        case ELogLevel::Info:
            wprintf(TEXT("%ls"), OutputDeviceColor::COLOR_WHITE);
            break;

        case ELogLevel::Warning:
            wprintf(TEXT("%ls"), OutputDeviceColor::COLOR_YELLOW);
            break;

        case ELogLevel::Error:
            wprintf(TEXT("%ls"), OutputDeviceColor::COLOR_RED);
            break;

        case ELogLevel::Fatal:
            wprintf(TEXT("%ls"), OutputDeviceColor::COLOR_NORMAL);
            wprintf(TEXT("%ls"), OutputDeviceColor::BACKGROUND_RED);
            wprintf(TEXT("%ls"), OutputDeviceColor::BOLD);
            break;

        case ELogLevel::Debug:
        default:
            wprintf(TEXT("%ls"), OutputDeviceColor::COLOR_NORMAL);
            break;
    }

    std::time_t time = std::time(nullptr);
    wprintf(TEXT("[%ls] %ls: %ls > %ls%ls\n"), ANSI_TO_TCHAR(TimeStamp().c_str()), toString(level), *categoryName, message, OutputDeviceColor::COLOR_NORMAL);
}

DEFINE_SINGLETON(Logger)