#include "AssertionMacros.h"

#include "Logging/Logger.h"
#include "Container/String.h"

void Assertion::CheckVerifyFailed(const char *file, size_t line, const TCHAR *format, ...) {
    va_list ap;
            va_start(ap, format);
    auto message = String::Vprintf(format, ap);
            va_end(ap);

    auto output = String::Printf(TEXT("%ls at %ls:%d"), *message, ANSI_TO_TCHAR(file), line);
    Logger::Get().log(TEXT("Assertion failed"), ELogLevel::Fatal, *message);
}

