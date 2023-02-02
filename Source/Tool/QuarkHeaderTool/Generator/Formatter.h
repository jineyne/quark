#pragma once

#include "QHTPrerequisites.h"
#include "FileSystem/Stream.h"

struct FNamedFormatterArgs {
public:
    TMap<FString, FString> args;

public:
    template<typename T>
    FNamedFormatterArgs &add(FString key, T arg) {
        args.add(key, FString::ToString(arg));
        return *this;
    }

    template<>
    FNamedFormatterArgs &add(FString key, const TCHAR *arg) {
        args.add(key, FString(arg));
        return *this;
    }

    template<>
    FNamedFormatterArgs &add(FString key, FString arg) {
        args.add(key, (FString) arg);
        return *this;
    }
};

class FFormatter {
private:
    TSharedPtr<FStream> mStream;
    FArchive *mArchive;

    FString mIndent = "";
    int32_t mIndentLevel = 0;

public:
    FFormatter(const TSharedPtr<FStream> &stream = nullptr);
    ~FFormatter();

public:
    void append(const FString &str, const FNamedFormatterArgs &namedArgs = FNamedFormatterArgs(), bool bPushIndent = false);
    void appendLine(const FString &str = "", const FNamedFormatterArgs &namedArgs = FNamedFormatterArgs(), bool bPushIndent = false);

    void addIndent(int level = 1);
    void removeIndent(int level = 1);
};