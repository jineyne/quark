#include "LogMacros.h"
#include "Container/String.h"
#include "Logger.h"

#define LOG_CASE(LEVEL) case ELogLevel::LEVEL: return TEXT(#LEVEL)

const TCHAR *toString(ELogLevel level) {
    switch (level) {
        LOG_CASE(NoLogging);
        LOG_CASE(Debug);
        LOG_CASE(Fatal);
        LOG_CASE(Error);
        LOG_CASE(Warning);
        LOG_CASE(Info);
        default:
            check(false);
            return TEXT("Unknown Log Level");
    }
}

void FMsg::PrintLog(const ANSICHAR *file, size_t line, const FString &category, ELogLevel level, const TCHAR *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    FLogger::Get().log(category, level, *FString::Vprintf(fmt, ap));
    va_end(ap);
}
