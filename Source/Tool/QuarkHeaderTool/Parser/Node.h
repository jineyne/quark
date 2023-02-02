#pragma once

#include "QHTPrerequisites.h"

#include "Token.h"

#define TO_STATEMENT(P) reinterpret_cast<FStatementNode *>(P)
#define TO_DECLARE(P) reinterpret_cast<FDeclareNode *>(P)
#define TO_FIELD(P) reinterpret_cast<FFieldNode *>(P)

#define TO_PSTATEMENT(P) reinterpret_cast<FStatementNode **>(&P)
#define TO_PDECLARE(P) reinterpret_cast<FDeclareNode **>(&P)
#define TO_PFIELD(P) reinterpret_cast<FFieldNode **>(&P)

enum class ENodeType {
    Compound,
    Namespace,

    Directive,
    CustomMacro,
    EmptyStatement,

    Declare,
    VariableDeclare,
    FunctionDeclare,
    StructDeclare,
    ClassDeclare,
    EnumDeclare,

    Field,
    Property,
    EnumField,

    Expression,
    Type,
    LiteralType,
    TemplateType,
    ReferenceType,
    LReferenceType,
    PointerType,
    FunctionPointerType,

    LValue,
    Literal,
};

class FNode {
public:
    bool isConst = false;
    bool isVolatile = false;
    bool isMutable = false;

    FToken token;
    ENodeType type;

public:
    FNode(ENodeType type);

    virtual ~FNode() = default;

public:
    virtual bool isStatement() const { return false; }

    virtual bool isDeclare() const { return false; }

    virtual bool isField() const { return false; }

    virtual bool isExpression() const { return false; }

    virtual bool isType() const { return false; }
};

class FStatementNode : public FNode {
public:
    TMap<FString, FString> meta;
    bool bHasPrefixMacro = false;

public:
    FStatementNode(ENodeType type);

public:
    bool isStatement() const override { return true; }
};

class FExpressionNode : public FNode {
public:
    FExpressionNode(ENodeType type);

public:
    bool isExpression() const override { return true; }
};

class FTypeNode : public FExpressionNode {
public:
    bool bUnSigned = false;

public:
    FTypeNode(ENodeType type);

public:
    bool isType() const override { return true; }
};

class FLValue : public FExpressionNode {
public:
    FLValue(ENodeType type);
};

class FFieldNode : public FStatementNode {
public:
    FToken dataType;
    EAccessControlType access;
    FExpressionNode *initializer = nullptr;

public:
    FFieldNode(ENodeType type);

    ~FFieldNode();

public:
    bool isField() const override { return true; }
};

class FDeclareNode : public FStatementNode {
public:
    FTypeNode *base = nullptr;
    EAccessControlType access;
    TArray<FStatementNode *> fields;

public:
    FDeclareNode(ENodeType type);

    virtual ~FDeclareNode();

public:
    bool isDeclare() const override { return true; }
};

class FArgument {
public:
    FString name = "";
    FTypeNode *type = nullptr;
    FExpressionNode *initializer = nullptr;

public:
    ~FArgument();
};

#pragma region Expression Node List

class FLiteralTypeNode : public FTypeNode {
public:
    FLiteralTypeNode();
};

class FTemplateTypeNode : public FTypeNode {
public:
    TArray<FTypeNode *> arguments{};

public:
    FTemplateTypeNode();

    ~FTemplateTypeNode();
};

class FReferenceType : public FTypeNode {
public:
    FTypeNode *base = nullptr;
public:
    FReferenceType();

    ~FReferenceType();
};

class FLReferenceType : public FTypeNode {
public:
    FTypeNode *base = nullptr;
public:
    FLReferenceType();

    ~FLReferenceType();
};

class FPointerType : public FTypeNode {
public:
    FTypeNode *base = nullptr;

public:
    FPointerType();

    ~FPointerType();
};

class FFunctionPointerType : public FTypeNode {
public:
    FTypeNode *base = nullptr;

    TArray<FArgument *> arguments;
    FTypeNode *returns = nullptr;

public:
    FFunctionPointerType();

    ~FFunctionPointerType();
};

class FLiteralNode : public FLValue {
public:
    FLiteral literal;

public:
    FLiteralNode();
};

#pragma endregion

#pragma region Statement Node List

class FCompoundNode : public FStatementNode {
public:
    TArray<FStatementNode *> statements;

public:
    FCompoundNode();

    FCompoundNode(ENodeType type);

    virtual ~FCompoundNode();
};

class FNamespaceNode : public FCompoundNode {
public:
    FNamespaceNode();
};

class FDirectiveNode : public FStatementNode {
public:
    FToken data;

public:
    FDirectiveNode();
};

class FCustomMacroNode : public FStatementNode {
public:
    FCustomMacroNode();
};

class FEmptyStatementNode : public FStatementNode {
public:
    FEmptyStatementNode();
};

class FVariableDeclareNode : public FDeclareNode {

public:
    FVariableDeclareNode();
};

class FFunctionDeclareNode : public FDeclareNode {
public:
    TArray<FArgument *> arguments;
    FTypeNode *returns;
    bool isMacro = false;

public:
    FFunctionDeclareNode();
};

class FStructDeclareNode : public FDeclareNode {
public:
    EAccessControlType baseAccess;

    FFunctionDeclareNode *constructor = nullptr;
    FFunctionDeclareNode *destructor = nullptr;
    FFunctionDeclareNode *generated = nullptr;

public:
    FStructDeclareNode();
};

class FClassDeclareNode : public FDeclareNode {
public:
    EAccessControlType baseAccess;

    FFunctionDeclareNode *constructor = nullptr;
    FFunctionDeclareNode *destructor = nullptr;
    FFunctionDeclareNode *generated = nullptr;

    TArray<FDeclareNode *> functions;

public:
    FClassDeclareNode();
};

class FEnumDeclareNode : public FDeclareNode {
public:
    bool cxxclass;

public:
    FEnumDeclareNode();
};

class FPropertyNode : public FFieldNode {
public:
    FTypeNode *dataType;

public:
    FPropertyNode();
};

class FEnumFieldNode : public FFieldNode {
public:
    FEnumFieldNode();
};

#pragma endregion