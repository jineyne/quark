#include "Logger.h"

#include <chrono>

#if PLATFORM == PLATFORM_WIN32
#include <Windows.h>
#endif

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

    const TCHAR * const BG_NORMAL = TEXT("\x1b[40m");
    const TCHAR * const BG_RED = TEXT("\x1b[41m");

    const TCHAR * const BOLD = TEXT("\x1b[1m");
    const TCHAR * const BOLD_OFF = TEXT("\x1b[22m");
}

enum class Color {
    Black,
    DarkBlue,
    DarkGreen,
    DarkCyan,
    DarkRed,
    DarkMagenta,
    DarkYellow,
    Gray,
    DarkGray,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Yellow,
    White,
    LightGray = 7,
    Normal = 7,
};

std::string TimeStamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%F %T");
    return ss.str();
}

Logger::Logger() {
}

#if PLATFORM == PLATFORM_WIN32

void setForeground(Color color) {
    CONSOLE_SCREEN_BUFFER_INFO info{};
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (info.wAttributes & 0xf0) | ((int) color & 0x0f));
}

void setBackground(Color color) {
    CONSOLE_SCREEN_BUFFER_INFO info{};
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((int) color & 0xf0) | (info.wAttributes & 0x0f));
}

#endif

void Logger::log(const String &categoryName, ELogLevel level, const TCHAR *message) {
    // TODO: Optimazation
#if PLATFORM == PLATFORM_WIN32
    switch (level) {
        case ELogLevel::Info:
            setForeground(Color::White);
            break;

        case ELogLevel::Warning:
            setForeground(Color::Yellow);
            break;

        case ELogLevel::Error:
            setForeground(Color::Red);
            break;

        case ELogLevel::Fatal:
            setForeground(Color::Normal);
            setBackground(Color::Red);
            break;

        case ELogLevel::Debug:
        default:
            setForeground(Color::Normal);
            break;
    }
#else
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
            wprintf(TEXT("%ls"), OutputDeviceColor::BG_RED);
            break;

        case ELogLevel::Debug:
        default:
            wprintf(TEXT("%ls"), OutputDeviceColor::COLOR_NORMAL);
            break;
    }
#endif

    std::time_t time = std::time(nullptr);
    wprintf(TEXT("[%ls] %ls: %ls > %ls\n"), ANSI_TO_TCHAR(TimeStamp().c_str()), toString(level), *categoryName, message);

#if COMPILER == COMPILER_MSVC

#endif

#if PLATFORM == PLATFORM_WIN32
    setForeground(Color::Normal);
    setBackground(Color::Black);
#else
    wprintf(TEXT("%ls"), OutputDeviceColor::COLOR_NORMAL);
#endif
}

DEFINE_SINGLETON(Logger)