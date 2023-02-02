#include "Formatter.h"

#include <Serialization/TextArchive.h>
#include <inja/inja.hpp>

static FString NewLine = TEXT("\n");

FFormatter::FFormatter(const TSharedPtr<FStream> &stream) : mStream(stream) {
    mArchive = new FTextArchive(mStream, EArchiveMode::Save);
}

FFormatter::~FFormatter() {
    delete mArchive;
}

void FFormatter::append(const FString &str, const FNamedFormatterArgs &namedArgs, bool bPushIndent) {
    if (bPushIndent) {
        *mArchive << mIndent;
    }

    const TMap<FString, FString> &args = TMap<FString,FString>(namedArgs.args);

    inja::json data;
    for (auto &arg : args) {
        if (arg.first.length() == 0) {
            continue;
        }

        if (arg.second.length() == 0) {
            data[TCHAR_TO_ANSI(*arg.first)] = "";
            continue;
        }

        data[TCHAR_TO_ANSI(*arg.first)] = std::string(TCHAR_TO_ANSI(*arg.second)).c_str();
    }

    FString formatted = ANSI_TO_TCHAR(inja::render(TCHAR_TO_ANSI(*str), data).c_str());
    *mArchive << formatted;
}

void FFormatter::appendLine(const FString &str, const FNamedFormatterArgs &namedArgs, bool bPushIndent) {
    if (bPushIndent) {
        *mArchive << mIndent;
    }

    const TMap<FString, FString> &args = TMap<FString,FString>(namedArgs.args);

    inja::json data;
    for (auto &arg : args) {
        if (arg.first.length() == 0) {
            continue;
        }

        if (arg.second.length() == 0) {
            data[TCHAR_TO_ANSI(*arg.first)] = "";
            continue;
        }

        data[TCHAR_TO_ANSI(*arg.first)] = std::string(TCHAR_TO_ANSI(*arg.second)).c_str();
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