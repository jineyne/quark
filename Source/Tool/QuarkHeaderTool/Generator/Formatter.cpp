#include "Formatter.h"

#include <Serialization/TextArchive.h>
#include <inja/inja.hpp>

static FString NewLine = TEXT("\n");

FFormatter::FFormatter(const TSharedPtr<FStream> &stream) : mStream(stream) {
    mArchive = q_new<FTextArchive>(mStream, EArchiveMode::Save);
}

FFormatter::~FFormatter() {
    delete mArchive;
}

void FFormatter::append(const FString &str, const FNamedFormatterArgs &namedArgs, bool bPushIndent) {
    if (bPushIndent) {
        *mArchive << mIndent;
    }

    const auto &args = TMap<FString, FString>(namedArgs.args);

    inja::json data;
    for (auto &arg : args) {
        if (arg.key.length() == 0) {
            continue;
        }

        if (arg.value.length() == 0) {
            data[TCHAR_TO_ANSI(*arg.key)] = "";
            continue;
        }

        data[TCHAR_TO_ANSI(*arg.key)] = std::string(TCHAR_TO_ANSI(*arg.value)).c_str();
    }

    FString formatted = ANSI_TO_TCHAR(inja::render(TCHAR_TO_ANSI(*str), data).c_str());
    *mArchive << formatted;
}

void FFormatter::appendLine(const FString &str, const FNamedFormatterArgs &namedArgs, bool bPushIndent) {
    if (bPushIndent) {
        *mArchive << mIndent;
    }

    const auto &args = TMap<FString, FString>(namedArgs.args);

    inja::json data;
    for (auto &arg : args) {
        if (arg.key.length() == 0) {
            continue;
        }

        if (arg.value.length() == 0) {
            data[TCHAR_TO_ANSI(*arg.key)] = "";
            continue;
        }

        data[TCHAR_TO_ANSI(*arg.key)] = std::string(TCHAR_TO_ANSI(*arg.value)).c_str();
    }

    FString formatted = ANSI_TO_TCHAR(inja::render(TCHAR_TO_ANSI(*str), data).c_str());
    *mArchive << formatted << NewLine;
}

void FFormatter::addIndent(int level) {
    mIndentLevel += level;
    mIndent.clear();
    for (size_t i = 0; i < mIndentLevel; i++) {
        mIndent += TEXT("    ");
    }
}

void FFormatter::removeIndent(int level) {
    check(mIndentLevel - level >= 0);

    mIndentLevel -= level;
    mIndent.clear();
    for (int32_t i = 0; i < mIndentLevel; i++) {
        mIndent += TEXT("    ");
    }
}
