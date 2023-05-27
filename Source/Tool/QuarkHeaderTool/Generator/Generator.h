#pragma once

#include "QHTPrerequisites.h"

#include "FileSystem/Path.h"
#include "Reflection/Reflection.h"

#include "Parser/NodeVisiter.h"
#include "Formatter.h"

struct GeneratorDesc {
    Path path;
    Path relativePath;
    String package;

    TSharedPtr<Stream> source;
    TSharedPtr<Stream> header;
};

class Generator : public INodeVisiter<Literal>  {
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

        String fullName;
        String currentName;
        EScopeType type;
    };

    FScope *mTopScope = nullptr;

    Formatter mHeaderFormatter;
    Formatter mSourceFormatter;

    Path mPath;
    Path mRelativePath;
    String mPackage = "";
    String mCurrentFileId = "";

    TMap<String, String> mMetaData;
    Literal mUserData;

public:
    Generator(const Path &path);
    Generator(const TSharedPtr<Stream> &source, const TSharedPtr<Stream> &header, const Path &path);
    Generator(const GeneratorDesc &desc);

public:
    bool generate(Node *node);

protected:
    Literal visitNamespace(NamespaceNode *node) override;

    Literal visitDirective(DirectiveNode *node) override;

    Literal visitVariableDeclare(VariableDeclareNode *node) override;
    Literal visitFunctionDeclare(FunctionDeclareNode *node) override;
    Literal visitStructDeclare(StructDeclareNode *node) override;
    Literal visitClassDeclare(ClassDeclareNode *node) override;
    Literal visitEnumDeclare(EnumDeclareNode *node) override;

    Literal visitProperty(PropertyNode *node) override;
    Literal visitEnumField(EnumFieldNode *node) override;

    Literal visitLiteral(LiteralNode *node) override;

    void pushScope(const String &name, FScope::EScopeType type);
    void popScope();

    void printGenerateBody(ClassDeclareNode *node, FunctionDeclareNode *generated);
    void printGenerateStructBody(StructDeclareNode *node, FunctionDeclareNode *generated);


private:
    void printStatics(DeclareNode *node, FunctionDeclareNode *generated, FScope::EScopeType scope);
    Reflection::EPropertyGenFlags getDataType(TypeNode *node);
    void renderPropertyType(TypeNode *node);
};
