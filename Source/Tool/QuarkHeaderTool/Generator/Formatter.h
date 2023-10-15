#pragma once

#include "QHTPrerequisites.h"

struct NamedFormatterArgs {
public:
    std::map<std::string, std::string> args;

public:
    template<typename T>
    NamedFormatterArgs &add(std::string key, T arg) {
        args.insert(std::make_pair(key, std::to_string(arg)));
        return *this;
    }

    template<>
    NamedFormatterArgs &add(std::string key, const char *arg) {
        args.insert(std::make_pair(key, std::string(arg)));
        return *this;
    }

    template<>
    NamedFormatterArgs &add(std::string key, std::string arg) {
        args.insert(std::make_pair(key, (std::string) arg));
        return *this;
    }
};

class Formatter {
private:
    std::shared_ptr<std::ofstream> mStream;

    std::string mIndent = "";
    int32_t mIndentLevel = 0;

public:
    Formatter(const std::shared_ptr<std::ofstream> &stream = nullptr);
    ~Formatter();

public:
    void append(const std::string &str, const NamedFormatterArgs &namedArgs = NamedFormatterArgs(), bool bPushIndent = false);
    void appendLine(const std::string &str = "", const NamedFormatterArgs &namedArgs = NamedFormatterArgs(), bool bPushIndent = false);

    void addIndent(int level = 1);
    void removeIndent(int level = 1);
};