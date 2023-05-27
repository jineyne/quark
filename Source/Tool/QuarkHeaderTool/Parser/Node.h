#pragma once

#include "QHTPrerequisites.h"

#include "Token.h"

#define TO_STATEMENT(P) reinterpret_cast<FStatementNode *>(P)
#define TO_DECLARE(P) reinterpret_cast<FDeclareNode *>(P)
#define TO_FIELD(P) reinterpret_cast<FieldNode *>(P)

#define TO_PSTATEMENT(P) reinterpret_cast<StatementNode **>(&P)
#define TO_PDECLARE(P) reinterpret_cast<FDeclareNode **>(&P)
#define TO_PFIELD(P) reinterpret_cast<FFieldNode **>(&P)

enum class ENodeType {
    Compound,
    Namespace,

    Directive,
    CustomMacro,
    EmptyStatement,
    FunctionCall,

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

class Node {
public:
    bool isConst = false;
    bool isVolatile = false;
    bool isMutable = false;

    Token token;
    ENodeType type;

public:
    Node(ENodeType type);

    virtual ~Node() = default;

public:
    virtual bool isStatement() const { return false; }

    virtual bool isDeclare() const { return false; }

    virtual bool isField() const { return false; }

    virtual bool isExpression() const { return false; }

    virtual bool isType() const { return false; }
};

class StatementNode : public Node {
public:
    TMap<String, String> meta;
    bool bHasPrefixMacro = false;

public:
    StatementNode(ENodeType type);

public:
    bool isStatement() const override { return true; }
};

class ExpressionNode : public Node {
public:
    ExpressionNode(ENodeType type);

public:
    bool isExpression() const override { return true; }
};

class TypeNode : public ExpressionNode {
public:
    bool bUnSigned = false;

public:
    TypeNode(ENodeType type);

public:
    bool isType() const override { return true; }
};

class LValue : public ExpressionNode {
public:
    LValue(ENodeType type);
};

class FieldNode : public StatementNode {
public:
    Token dataType;
    EAccessControlType access;
    ExpressionNode *initializer = nullptr;

public:
    FieldNode(ENodeType type);

    ~FieldNode();

public:
    bool isField() const override { return true; }
};

class DeclareNode : public StatementNode {
public:
    TypeNode *base = nullptr;
    EAccessControlType access;
    TArray<StatementNode *> fields;

public:
    DeclareNode(ENodeType type);

    virtual ~DeclareNode();

public:
    bool isDeclare() const override { return true; }
};

class Argument {
public:
    String name = "";
    TypeNode *type = nullptr;
    ExpressionNode *initializer = nullptr;

public:
    ~Argument();
};

#pragma region Expression Node List

class LiteralTypeNode : public TypeNode {
public:
    LiteralTypeNode();
};

class TemplateTypeNode : public TypeNode {
public:
    TArray<TypeNode *> arguments{};

public:
    TemplateTypeNode();

    ~TemplateTypeNode();
};

class ReferenceType : public TypeNode {
public:
    TypeNode *base = nullptr;
public:
    ReferenceType();

    ~ReferenceType();
};

class LReferenceType : public TypeNode {
public:
    TypeNode *base = nullptr;
public:
    LReferenceType();

    ~LReferenceType();
};

class PointerType : public TypeNode {
public:
    TypeNode *base = nullptr;

public:
    PointerType();

    ~PointerType();
};

class FunctionPointerType : public TypeNode {
public:
    TypeNode *base = nullptr;

    TArray<Argument *> arguments;
    TypeNode *returns = nullptr;

public:
    FunctionPointerType();

    ~FunctionPointerType();
};

class LiteralNode : public LValue {
public:
    Literal literal;

public:
    LiteralNode();
};

#pragma endregion

#pragma region Statement Node List

class CompoundNode : public StatementNode {
public:
    TArray<StatementNode *> statements;

public:
    CompoundNode();

    CompoundNode(ENodeType type);

    virtual ~CompoundNode();
};

class NamespaceNode : public CompoundNode {
public:
    NamespaceNode();
};

class FunctionCallNode : public StatementNode {
public:
    FunctionCallNode();
};

class DirectiveNode : public StatementNode {
public:
    Token data;

public:
    DirectiveNode();
};

class CustomMacroNode : public StatementNode {
public:
    CustomMacroNode();
};

class EmptyStatementNode : public StatementNode {
public:
    EmptyStatementNode();
};

class VariableDeclareNode : public DeclareNode {

public:
    VariableDeclareNode();
};

class FunctionDeclareNode : public DeclareNode {
public:
    TArray<Argument *> arguments;
    TypeNode *returns;
    bool isMacro = false;

public:
    FunctionDeclareNode();
};

class StructDeclareNode : public DeclareNode {
public:
    EAccessControlType baseAccess;

    FunctionDeclareNode *constructor = nullptr;
    FunctionDeclareNode *destructor = nullptr;
    FunctionDeclareNode *generated = nullptr;

public:
    StructDeclareNode();
};

class ClassDeclareNode : public DeclareNode {
public:
    EAccessControlType baseAccess;

    FunctionDeclareNode *constructor = nullptr;
    FunctionDeclareNode *destructor = nullptr;
    FunctionDeclareNode *generated = nullptr;

    TArray<DeclareNode *> functions;

public:
    ClassDeclareNode();
};

class EnumDeclareNode : public DeclareNode {
public:
    bool cxxclass;

public:
    EnumDeclareNode();
};

class PropertyNode : public FieldNode {
public:
    TypeNode *dataType;

public:
    PropertyNode();
};

class EnumFieldNode : public FieldNode {
public:
    EnumFieldNode();
};

#pragma endregion