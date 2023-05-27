#pragma once

#include "QHTPrerequisites.h"
#include "Symbol.h"
#include "Token.h"
#include "Tokenzier.h"

class SymbolParser : public Tokenizer {
public:
    struct Options {
        String apiMacro = TEXT("DLL_EXPORT");
        String structNameMacro = TEXT("QSTRUCT");
        String classNameMacro = TEXT("QCLASS");
        String enumNameMacro = TEXT("QENUM");
        String enumEntryNameMacro = TEXT("QENTRY");
        String propertyNameMacro = TEXT("QPROPERTY");
        String methodNameMacro = TEXT("QFUNCTION");

        String generatedMacro = TEXT("GENERATED_BODY");

        TArray<String> customFunctionMacros = {TEXT("GENERATED_BODY"), TEXT("DECLARE_LOG_CATEGORY_EXTERN") };
        TArray<String> customMacros;
    };

    enum class EScopeType {
        Global,
        Enum,
        Namespace,
        Struct,
        Class,
    };

    struct Scope {
        String name;
        EScopeType type;
        EAccessControlType currentAccessControlType;
    };

private:
    Options mOptions;

    std::array<Scope, 64> mScopeList;
    Scope *mTopScope;
    size_t mScopeIndex = 0;

    TArray<Symbol *> mSymbols;

public:
    SymbolParser(Options options);
    virtual ~SymbolParser() = default;

public:
    TArray<Symbol *> *run(const String &input);

private:
    bool statement();
    bool declaration(Token &token);

    bool enum_(Token &token);
    bool enumEntry(Token &token);
    bool struct_(Token &token);
    bool class_(Token &token);

    bool property(Token &token);
    bool method(Token &token);

    bool meta(Token &token, String macro, Symbol *target);
    bool metaSequence(Token &token, String scope, Symbol *target);

    bool type();
};