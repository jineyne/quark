#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"

enum class ELogLevel {
    NoLogging = 0,

    Debug,
    Fatal,
    Error,
    Warning,
    Info,

    All,
    LevelMask = 0xf,
};

DLL_EXPORT const TCHAR *toString(ELogLevel level);

struct FLogMeta {
    ELogLevel mLevel;
    String mTag;
};

class DLL_EXPORT LogMsg {
public:
    static void PrintLog(const ANSICHAR* file, size_t line, const String& category, ELogLevel level, const TCHAR* fmt, ...);
};

#define DECLARE_LOG_CATEGORY_EXTERN(CATEGORY_NAME, DefaultLevel) \
    extern struct LogCategory##CATEGORY_NAME : public LogCategory {\
        FORCEINLINE LogCategory##CATEGORY_NAME() : LogCategory(TEXT(#CATEGORY_NAME), ELogLevel::DefaultLevel) {} \
    } CATEGORY_NAME;

#define DEFINE_LOG_CATEGORY(CATEGORY_NAME) LogCategory##CATEGORY_NAME CATEGORY_NAME;

#define LOG(CATEGORY_NAME, LEVEL, FORMAT, ...) \
{ \
    if ((uint8_t) ELogLevel::LEVEL < (uint8_t) ELogLevel::All) { \
        LogMsg::PrintLog(__FILE__, __LINE__, CATEGORY_NAME.toString(), ELogLevel::LEVEL, FORMAT, ##__VA_ARGS__);  \
    } \
}
