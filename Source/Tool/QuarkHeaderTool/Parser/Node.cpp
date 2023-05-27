#include "Node.h"

Node::Node(ENodeType type) : type(type) {}

StatementNode::StatementNode(ENodeType type) : Node(type) {}

ExpressionNode::ExpressionNode(ENodeType type) : Node(type) {}

TypeNode::TypeNode(ENodeType type) : ExpressionNode(type) {}

LValue::LValue(ENodeType type) : ExpressionNode(type) {}

FieldNode::FieldNode(ENodeType type) : StatementNode(type) {}

FieldNode::~FieldNode() {
    delete initializer;
}

DeclareNode::DeclareNode(ENodeType type) : StatementNode(type) {}

DeclareNode::~DeclareNode() {
    for (auto field : fields) {
        delete field;
    }
}

Argument::~Argument() {
    delete type;
    delete initializer;
}

LiteralTypeNode::LiteralTypeNode() : TypeNode(ENodeType::LiteralType) {}

TemplateTypeNode::TemplateTypeNode() : TypeNode(ENodeType::TemplateType) {}

TemplateTypeNode::~TemplateTypeNode() {
    for (auto argument: arguments) {
        delete argument;
    }
}

ReferenceType::ReferenceType() : TypeNode(ENodeType::ReferenceType) {}

ReferenceType::~ReferenceType() {
    delete base;
}

LReferenceType::LReferenceType() : TypeNode(ENodeType::LReferenceType) {}

LReferenceType::~LReferenceType() {
    delete base;
}

PointerType::PointerType() : TypeNode(ENodeType::PointerType) {}

PointerType::~PointerType() {
    delete base;
}

FunctionPointerType::FunctionPointerType() : TypeNode(ENodeType::FunctionPointerType) {}

FunctionPointerType::~FunctionPointerType() {
    delete base;
    for (auto argument: arguments) {
        delete argument;
    }
    delete returns;
}

LiteralNode::LiteralNode() : LValue(ENodeType::Literal) {}

CompoundNode::CompoundNode() : StatementNode(ENodeType::Compound) {}

CompoundNode::CompoundNode(ENodeType type) : StatementNode(type) {}

CompoundNode::~CompoundNode() {
    for (auto statement: statements) {
        delete statement;
    }
}

NamespaceNode::NamespaceNode() : CompoundNode(ENodeType::Namespace) {}

FunctionCallNode::FunctionCallNode() : StatementNode(ENodeType::FunctionCall) {}

DirectiveNode::DirectiveNode() : StatementNode(ENodeType::Directive) {}

CustomMacroNode::CustomMacroNode() : StatementNode(ENodeType::CustomMacro) {}

EmptyStatementNode::EmptyStatementNode() : StatementNode(ENodeType::EmptyStatement) {}

VariableDeclareNode::VariableDeclareNode() : DeclareNode(ENodeType::VariableDeclare) {}

FunctionDeclareNode::FunctionDeclareNode() : DeclareNode(ENodeType::FunctionDeclare) {}

StructDeclareNode::StructDeclareNode() : DeclareNode(ENodeType::StructDeclare) {}

ClassDeclareNode::ClassDeclareNode() : DeclareNode(ENodeType::ClassDeclare) {}

EnumDeclareNode::EnumDeclareNode() : DeclareNode(ENodeType::EnumDeclare) {}

PropertyNode::PropertyNode() : FieldNode(ENodeType::Property) {}

EnumFieldNode::EnumFieldNode() : FieldNode(ENodeType::EnumField) {}
