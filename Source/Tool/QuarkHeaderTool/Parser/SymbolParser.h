#pragma once

#include "QHTPrerequisites.h"
#include "Symbol.h"
#include "Token.h"
#include "Tokenzier.h"

class FSymbolParser : public Tokenizer {
public:
    struct FOptions {
        FString apiMacro = TEXT("DLL_EXPORT");
        FString structNameMacro = TEXT("QSTRUCT");
        FString classNameMacro = TEXT("QCLASS");
        FString enumNameMacro = TEXT("QENUM");
        FString enumEntryNameMacro = TEXT("QENTRY");
        FString propertyNameMacro = TEXT("QPROPERTY");
        FString methodNameMacro = TEXT("QFUNCTION");

        FString generatedMacro = TEXT("GENERATED_BODY");

        TArray<FString> customFunctionMacros = { TEXT("GENERATED_BODY"), TEXT("DECLARE_LOG_CATEGORY_EXTERN") };
        TArray<FString> customMacros;
    };

    enum class EScopeType {
        Global,
        Enum,
        Namespace,
        Struct,
        Class,
    };

    struct FScope {
        FString name;
        EScopeType type;
        EAccessControlType currentAccessControlType;
    };

private:
    FOptions mOptions;

    std::array<FScope, 64> mScopeList;
    FScope *mTopScope;
    size_t mScopeIndex = 0;

    TArray<FSymbol *> mSymbols;

public:
    FSymbolParser(FOptions options);
    virtual ~FSymbolParser() = default;

public:
    TArray<FSymbol *> *run(const FString &input);

private:
    bool statement();
    bool declaration(FToken &token);

    bool enum_(FToken &token);
    bool enumEntry(FToken &token);
    bool struct_(FToken &token);
    bool class_(FToken &token);

    bool property(FToken &token);
    bool method(FToken &token);

    bool meta(FToken &token, FString macro, FSymbol *target);
    bool metaSequence(FToken &token, FString scope, FSymbol *target);

    bool type();
};