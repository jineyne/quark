#pragma once

#include "QHTPrerequisites.h"
#include "FileSystem/Stream.h"

struct NamedFormatterArgs {
public:
    TMap<String, String> args;

public:
    template<typename T>
    NamedFormatterArgs &add(String key, T arg) {
        args.add(key, String::ToString(arg));
        return *this;
    }

    template<>
    NamedFormatterArgs &add(String key, const TCHAR *arg) {
        args.add(key, String(arg));
        return *this;
    }

    template<>
    NamedFormatterArgs &add(String key, String arg) {
        args.add(key, (String) arg);
        return *this;
    }
};

class Formatter {
private:
    TSharedPtr<Stream> mStream;
    Archive *mArchive;

    String mIndent = "";
    int32_t mIndentLevel = 0;

public:
    Formatter(const TSharedPtr<Stream> &stream = nullptr);
    ~Formatter();

public:
    void append(const String &str, const NamedFormatterArgs &namedArgs = NamedFormatterArgs(), bool bPushIndent = false);
    void appendLine(const String &str = "", const NamedFormatterArgs &namedArgs = NamedFormatterArgs(), bool bPushIndent = false);

    void addIndent(int level = 1);
    void removeIndent(int level = 1);
};