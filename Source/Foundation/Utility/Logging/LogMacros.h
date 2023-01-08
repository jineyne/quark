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
    FString mTag;
};

class DLL_EXPORT FMsg {
public:
    static void PrintLog(const ANSICHAR* file, size_t line, const FString& category, ELogLevel level, const TCHAR* fmt, ...);
};

#define DECLARE_LOG_CATEGORY_EXTERN(CATEGORY_NAME, DefaultLevel) \
    extern struct FLogCategory##CATEGORY_NAME : public FLogCategory {\
        FORCEINLINE FLogCategory##CATEGORY_NAME() : FLogCategory(TEXT(#CATEGORY_NAME), ELogLevel::DefaultLevel) {} \
    } CATEGORY_NAME;

#define DEFINE_LOG_CATEGORY(CATEGORY_NAME) FLogCategory##CATEGORY_NAME CATEGORY_NAME;

#define LOG(CATEGORY_NAME, LEVEL, FORMAT, ...)                                                               \
{                                                                                                               \
    if ((uint8_t) ELogLevel::LEVEL < (uint8_t) ELogLevel::All) {                                                \
        FMsg::PrintLog(__FILE__, __LINE__, CATEGORY_NAME.toString(), ELogLevel::LEVEL, FORMAT, ##__VA_ARGS__);  \
    }                                                                                                           \
}
