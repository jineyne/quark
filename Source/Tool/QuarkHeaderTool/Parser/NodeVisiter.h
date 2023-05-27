#pragma once

#include "QHTPrerequisites.h"
#include "Node.h"

template<typename ReturnType>
class INodeVisiter {
public:
    ReturnType visit(Node *node) {
#define TO(DST, SRC) reinterpret_cast<DST *>((SRC))
        switch (node->type) {
            case ENodeType::Compound:
                return visitCompound(TO(CompoundNode, node));

            case ENodeType::Namespace:
                return visitNamespace(TO(NamespaceNode, node));

            case ENodeType::Directive:
                return visitDirective(TO(DirectiveNode, node));

            case ENodeType::VariableDeclare:
                return visitVariableDeclare(TO(VariableDeclareNode, node));

            case ENodeType::FunctionDeclare:
                return visitFunctionDeclare(TO(FunctionDeclareNode, node));

            case ENodeType::StructDeclare:
                return visitStructDeclare(TO(StructDeclareNode, node));

            case ENodeType::ClassDeclare:
                return visitClassDeclare(TO(ClassDeclareNode, node));

            case ENodeType::EnumDeclare:
                return visitEnumDeclare(TO(EnumDeclareNode, node));

            case ENodeType::Property:
                return visitProperty(TO(PropertyNode, node));

            case ENodeType::EnumField:
                return visitEnumField(TO(EnumFieldNode, node));

            case ENodeType::Literal:
                return visitLiteral(TO(LiteralNode, node));

            default:
                return ReturnType{};
        }
#undef TO
    }

protected:
    virtual ReturnType visitCompound(CompoundNode *node) {
        for (auto statement: node->statements) {
            visit(statement);
        }

        return ReturnType{};
    }

    virtual ReturnType visitNamespace(NamespaceNode *node) {
        for (auto statement: node->statements) {
            visit(statement);
        }

        return ReturnType{};
    }

    virtual ReturnType visitDirective(DirectiveNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitVariableDeclare(VariableDeclareNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitFunctionDeclare(FunctionDeclareNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitStructDeclare(StructDeclareNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitClassDeclare(ClassDeclareNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitEnumDeclare(EnumDeclareNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitProperty(PropertyNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitEnumField(EnumFieldNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitLiteral(LiteralNode *node) {
        return ReturnType{};
    }
};