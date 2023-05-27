#pragma once

#include "QHTPrerequisites.h"

#include "Node.h"
#include "Tokenzier.h"

struct Options {
    String apiMacro = TEXT("DLL_EXPORT");
    String structNameMacro = TEXT("QSTRUCT");
    String classNameMacro = TEXT("QCLASS");
    String enumNameMacro = TEXT("QENUM");
    String enumEntryNameMacro = TEXT("QENTRY");
    String propertyNameMacro = TEXT("QPROPERTY");
    String functionNameMacro = TEXT("QFUNCTION");

    String generatedMacro = TEXT("GENERATED_BODY");

    TArray<String> customFunctionMacros = {TEXT("GENERATED_BODY"), TEXT("DECLARE_LOG_CATEGORY_EXTERN") };
    TArray<String> customMacros;
};

enum class EScopeType {
    Global,
    Namespace,
    Struct,
    Class,
};

struct Scope {
    String name;
    EScopeType type;
    EAccessControlType currentAccessControlType;
};

class Parser : public Tokenizer {
private:
    Options mOptions;

    std::array<Scope, 64> mScopeList;
    Scope *mTopScope;
    size_t mScopeIndex = 0;

public:
    Parser(Options options);

    virtual ~Parser() = default;

public:
    CompoundNode *parse(const String &input);

protected:
    StatementNode *parseStatement();

    StatementNode *parseDeclaration(Token &token);

    NamespaceNode *parseNamespace();

    DirectiveNode *parseDirective();

    CustomMacroNode *parseCustomMacro(Token &token);

    FunctionCallNode *parseFunctionCall(Token &token, bool bIsMacro = false);

    FunctionDeclareNode *parseFunctionDeclare(Token &token, bool bNeedReturnType = true, bool bIsMacro = false);

    StructDeclareNode *parseStruct(Token &token);

    ClassDeclareNode *parseClass(Token &token);

    EnumDeclareNode *parseEnum(Token &token);

    PropertyNode *parseProperty(Token &token);

    EnumFieldNode *parseEnumField(Token &token);

    TypeNode *parseType();

    bool parseMacroMeta(Token &token, String macroName, StatementNode **declare);

    bool parseMetaSequence(Token &token, String scope, StatementNode **declare);

    bool parseAccessControl(const Token &token, EAccessControlType &type);

    String parseTypeDeclarator();

    void pushScope(const String &name, EScopeType scopeType, EAccessControlType accessControlType);

    void popScope();

    void skipDeclaration(Token &token);
};