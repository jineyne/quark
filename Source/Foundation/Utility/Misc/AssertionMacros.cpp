#include "AssertionMacros.h"

#include "Logging/Logger.h"
#include "Container/String.h"

void FAssertion::CheckVerifyFailed(const char *file, size_t line, const TCHAR *format, ...) {
    va_list ap;
            va_start(ap, format);
    auto message = FString::Vprintf(format, ap);
            va_end(ap);

    auto output = FString::Printf(TEXT("%ls at %ls:%d"), *message, ANSI_TO_TCHAR(file), line);
    FLogger::Get().log(TEXT("Assertion failed"), ELogLevel::Fatal, *message);
}

