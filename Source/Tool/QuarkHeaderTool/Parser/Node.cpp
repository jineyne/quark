#include "Node.h"

FNode::FNode(ENodeType type) : type(type) {}

FStatementNode::FStatementNode(ENodeType type) : FNode(type) {}

FExpressionNode::FExpressionNode(ENodeType type) : FNode(type) {}

FTypeNode::FTypeNode(ENodeType type) : FExpressionNode(type) {}

FLValue::FLValue(ENodeType type) : FExpressionNode(type) {}

FFieldNode::FFieldNode(ENodeType type) : FStatementNode(type) {}

FFieldNode::~FFieldNode() {
    delete initializer;
}

FDeclareNode::FDeclareNode(ENodeType type) : FStatementNode(type) {}

FDeclareNode::~FDeclareNode() {
    for (auto field : fields) {
        delete field;
    }
}

FArgument::~FArgument() {
    delete type;
    delete initializer;
}

FLiteralTypeNode::FLiteralTypeNode() : FTypeNode(ENodeType::LiteralType) {}

FTemplateTypeNode::FTemplateTypeNode() : FTypeNode(ENodeType::TemplateType) {}

FTemplateTypeNode::~FTemplateTypeNode() {
    for (auto argument: arguments) {
        delete argument;
    }
}

FReferenceType::FReferenceType() : FTypeNode(ENodeType::ReferenceType) {}

FReferenceType::~FReferenceType() {
    delete base;
}

FLReferenceType::FLReferenceType() : FTypeNode(ENodeType::LReferenceType) {}

FLReferenceType::~FLReferenceType() {
    delete base;
}

FPointerType::FPointerType() : FTypeNode(ENodeType::PointerType) {}

FPointerType::~FPointerType() {
    delete base;
}

FFunctionPointerType::FFunctionPointerType() : FTypeNode(ENodeType::FunctionPointerType) {}

FFunctionPointerType::~FFunctionPointerType() {
    delete base;
    for (auto argument: arguments) {
        delete argument;
    }
    delete returns;
}

FLiteralNode::FLiteralNode() : FLValue(ENodeType::Literal) {}

FCompoundNode::FCompoundNode() : FStatementNode(ENodeType::Compound) {}

FCompoundNode::FCompoundNode(ENodeType type) : FStatementNode(type) {}

FCompoundNode::~FCompoundNode() {
    for (auto statement: statements) {
        delete statement;
    }
}

FNamespaceNode::FNamespaceNode() : FCompoundNode(ENodeType::Namespace) {}

FFunctionCallNode::FFunctionCallNode() : FStatementNode(ENodeType::FunctionCall) {}

FDirectiveNode::FDirectiveNode() : FStatementNode(ENodeType::Directive) {}

FCustomMacroNode::FCustomMacroNode() : FStatementNode(ENodeType::CustomMacro) {}

FEmptyStatementNode::FEmptyStatementNode() : FStatementNode(ENodeType::EmptyStatement) {}

FVariableDeclareNode::FVariableDeclareNode() : FDeclareNode(ENodeType::VariableDeclare) {}

FFunctionDeclareNode::FFunctionDeclareNode() : FDeclareNode(ENodeType::FunctionDeclare) {}

FStructDeclareNode::FStructDeclareNode() : FDeclareNode(ENodeType::StructDeclare) {}

FClassDeclareNode::FClassDeclareNode() : FDeclareNode(ENodeType::ClassDeclare) {}

FEnumDeclareNode::FEnumDeclareNode() : FDeclareNode(ENodeType::EnumDeclare) {}

FPropertyNode::FPropertyNode() : FFieldNode(ENodeType::Property) {}

FEnumFieldNode::FEnumFieldNode() : FFieldNode(ENodeType::EnumField) {}
