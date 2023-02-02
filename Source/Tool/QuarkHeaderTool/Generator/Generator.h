#pragma once

#include "QHTPrerequisites.h"

#include "FileSystem/Path.h"
#include "Reflection/Reflection.h"

#include "Parser/NodeVisiter.h"
#include "Formatter.h"

struct FGeneratorDesc {
    FPath path;
    FPath relativePath;
    FString package;

    TSharedPtr<FStream> source;
    TSharedPtr<FStream> header;
};

class FGenerator : public INodeVisiter<FLiteral>  {
private:
    struct FScope {
        enum class EScopeType {
            Global,
            Namespace,
            Class,
            Struct,
            Enum,
        };

        FScope *parent = nullptr;

        FString fullName;
        FString currentName;
        EScopeType type;
    };

    FScope *mTopScope = nullptr;

    FFormatter mHeaderFormatter;
    FFormatter mSourceFormatter;

    FPath mPath;
    FPath mRelativePath;
    FString mPackage = "";
    FString mCurrentFileId = "";

    TMap<FString, FString> mMetaData;
    FLiteral mUserData;

public:
    FGenerator(const FPath &path);
    FGenerator(const TSharedPtr<FStream> &source, const TSharedPtr<FStream> &header, const FPath &path);
    FGenerator(const FGeneratorDesc &desc);

public:
    bool generate(FNode *node);

protected:
    FLiteral visitNamespace(FNamespaceNode *node) override;

    FLiteral visitDirective(FDirectiveNode *node) override;

    FLiteral visitVariableDeclare(FVariableDeclareNode *node) override;
    FLiteral visitFunctionDeclare(FFunctionDeclareNode *node) override;
    FLiteral visitStructDeclare(FStructDeclareNode *node) override;
    FLiteral visitClassDeclare(FClassDeclareNode *node) override;
    FLiteral visitEnumDeclare(FEnumDeclareNode *node) override;

    FLiteral visitProperty(FPropertyNode *node) override;
    FLiteral visitEnumField(FEnumFieldNode *node) override;

    FLiteral visitLiteral(FLiteralNode *node) override;

    void pushScope(const FString &name, FScope::EScopeType type);
    void popScope();

    void printGenerateBody(FClassDeclareNode *node, FFunctionDeclareNode *generated);
    void printGenerateStructBody(FStructDeclareNode *node, FFunctionDeclareNode *generated);


private:
    void printStatics(FDeclareNode *node, FFunctionDeclareNode *generated, FScope::EScopeType scope);
    QReflection::EPropertyGenFlags getDataType(FTypeNode *node);
    FString nodeToPropertyName(FTypeNode *node);
};
