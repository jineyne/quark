#include "Formatter.h"

#include <Serialization/TextArchive.h>
#include <inja/inja.hpp>

static String NewLine = TEXT("\n");

Formatter::Formatter(const TSharedPtr<Stream> &stream) : mStream(stream) {
    mArchive = q_new<TextArchive>(mStream, EArchiveMode::Save);
}

Formatter::~Formatter() {
    delete mArchive;
}

void Formatter::append(const String &str, const NamedFormatterArgs &namedArgs, bool bPushIndent) {
    if (bPushIndent) {
        *mArchive << mIndent;
    }

    const auto &args = TMap<String, String>(namedArgs.args);

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

    String formatted = ANSI_TO_TCHAR(inja::render(TCHAR_TO_ANSI(*str), data).c_str());
    *mArchive << formatted;
}

void Formatter::appendLine(const String &str, const NamedFormatterArgs &namedArgs, bool bPushIndent) {
    if (bPushIndent) {
        *mArchive << mIndent;
    }

    const auto &args = TMap<String, String>(namedArgs.args);

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

    String formatted = ANSI_TO_TCHAR(inja::render(TCHAR_TO_ANSI(*str), data).c_str());
    *mArchive << formatted << NewLine;
}

void Formatter::addIndent(int level) {
    mIndentLevel += level;
    mIndent.clear();
    for (size_t i = 0; i < mIndentLevel; i++) {
        mIndent += TEXT("    ");
    }
}

void Formatter::removeIndent(int level) {
    check(mIndentLevel - level >= 0);

    mIndentLevel -= level;
    mIndent.clear();
    for (int32_t i = 0; i < mIndentLevel; i++) {
        mIndent += TEXT("    ");
    }
}
