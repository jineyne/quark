#pragma once

#include "QHTPrerequisites.h"

#include "Node.h"
#include "Tokenzier.h"

struct FOptions {
    FString apiMacro = TEXT("DLL_EXPORT");
    FString structNameMacro = TEXT("QSTRUCT");
    FString classNameMacro = TEXT("QCLASS");
    FString enumNameMacro = TEXT("QENUM");
    FString enumEntryNameMacro = TEXT("QENTRY");
    FString propertyNameMacro = TEXT("QPROPERTY");
    FString functionNameMacro = TEXT("QFUNCTION");

    FString generatedMacro = TEXT("GENERATED_BODY");

    TArray<FString> customFunctionMacros = { TEXT("GENERATED_BODY"), TEXT("DECLARE_LOG_CATEGORY_EXTERN") };
    TArray<FString> customMacros;
};

enum class EScopeType {
    Global,
    Namespace,
    Struct,
    Class,
};

struct FScope {
    FString name;
    EScopeType type;
    EAccessControlType currentAccessControlType;
};

class FParser : public Tokenizer {
private:
    FOptions mOptions;

    std::array<FScope, 64> mScopeList;
    FScope *mTopScope;
    size_t mScopeIndex = 0;

public:
    FParser(FOptions options);

    virtual ~FParser() = default;

public:
    FCompoundNode *parse(const FString &input);

protected:
    FStatementNode *parseStatement();

    FStatementNode *parseDeclaration(FToken &token);

    FNamespaceNode *parseNamespace();

    FDirectiveNode *parseDirective();

    FCustomMacroNode *parseCustomMacro(FToken &token);

    FFunctionCallNode *parseFunctionCall(FToken &token, bool bIsMacro = false);

    FFunctionDeclareNode *parseFunctionDeclare(FToken &token, bool bNeedReturnType = true, bool bIsMacro = false);

    FStructDeclareNode *parseStruct(FToken &token);

    FClassDeclareNode *parseClass(FToken &token);

    FEnumDeclareNode *parseEnum(FToken &token);

    FPropertyNode *parseProperty(FToken &token);

    FEnumFieldNode *parseEnumField(FToken &token);

    FTypeNode *parseType();

    bool parseMacroMeta(FToken &token, FString macroName, FStatementNode **declare);

    bool parseMetaSequence(FToken &token, FString scope, FStatementNode **declare);

    bool parseAccessControl(const FToken &token, EAccessControlType &type);

    FString parseTypeDeclarator();

    void pushScope(const FString &name, EScopeType scopeType, EAccessControlType accessControlType);

    void popScope();

    void skipDeclaration(FToken &token);
};