#pragma once

#include "QHTPrerequisites.h"
#include "Symbol.h"
#include "Token.h"
#include "Tokenzier.h"

class SymbolParser : public Tokenizer {
public:
    struct Options {
        std::string apiMacro = ("DLL_EXPORT");
        std::string structNameMacro = ("QSTRUCT");
        std::string classNameMacro = ("QCLASS");
        std::string enumNameMacro = ("QENUM");
        std::string enumEntryNameMacro = ("QENTRY");
        std::string propertyNameMacro = ("QPROPERTY");
        std::string methodNameMacro = ("QFUNCTION");

        std::string generatedMacro = ("GENERATED_BODY");

        std::vector<std::string> customFunctionMacros = {("GENERATED_BODY"), ("DECLARE_LOG_CATEGORY_EXTERN") };
        std::vector<std::string> customMacros;
    };

    enum class EScopeType {
        Global,
        Enum,
        Namespace,
        Struct,
        Class,
    };

    struct Scope {
        std::string name;
        EScopeType type;
        EAccessControlType currentAccessControlType;
    };

private:
    Options mOptions;

    std::array<Scope, 64> mScopeList;
    Scope *mTopScope;
    size_t mScopeIndex = 0;

    std::vector<Symbol *> mSymbols;

public:
    SymbolParser(Options options);
    virtual ~SymbolParser() = default;

public:
    std::vector<Symbol *> *run(const std::string &input);

private:
    bool statement();
    bool declaration(Token &token);

    bool enum_(Token &token);
    bool enumEntry(Token &token);
    bool struct_(Token &token);
    bool class_(Token &token);

    bool property(Token &token);
    bool method(Token &token);

    bool meta(Token &token, std::string macro, Symbol *target);
    bool metaSequence(Token &token, std::string scope, Symbol *target);

    bool type();
};