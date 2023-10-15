#include "Formatter.h"

#include <inja/inja.hpp>

static std::string NewLine = "\n";

Formatter::Formatter(const std::shared_ptr<std::ofstream> &stream) : mStream(stream) {
}

Formatter::~Formatter() {
}

void Formatter::append(const std::string &str, const NamedFormatterArgs &namedArgs, bool bPushIndent) {
    if (bPushIndent) {
        *mStream << mIndent;
    }

    const auto &args = std::map<std::string, std::string>(namedArgs.args);

    inja::json data;
    for (auto &arg : args) {
        if (arg.first.length() == 0) {
            continue;
        }

        if (arg.second.length() == 0) {
            data[arg.second] = "";
            continue;
        }

        data[arg.first] = std::string(arg.second);
    }

    std::string formatted = inja::render(str, data);
    *mStream << formatted;
}

void Formatter::appendLine(const std::string &str, const NamedFormatterArgs &namedArgs, bool bPushIndent) {
    if (bPushIndent) {
        *mStream << mIndent;
    }

    const auto &args = std::map<std::string, std::string>(namedArgs.args);

    inja::json data;
    for (auto &arg : args) {
        if (arg.first.length() == 0) {
            continue;
        }

        if (arg.second.length() == 0) {
            data[arg.first] = "";
            continue;
        }

        data[arg.first] = arg.second;
    }

    std::string formatted = inja::render(str, data);
    *mStream << formatted << NewLine;
}

void Formatter::addIndent(int level) {
    mIndentLevel += level;
    mIndent.clear();
    for (size_t i = 0; i < mIndentLevel; i++) {
        mIndent += "    ";
    }
}

void Formatter::removeIndent(int level) {
    assert(mIndentLevel - level >= 0);

    mIndentLevel -= level;
    mIndent.clear();
    for (int32_t i = 0; i < mIndentLevel; i++) {
        mIndent += "    ";
    }
}
