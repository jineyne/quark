#pragma once

#include "QHTPrerequisites.h"
#include "Node.h"

template<typename ReturnType>
class INodeVisiter {
public:
    ReturnType visit(FNode *node) {
#define TO(DST, SRC) reinterpret_cast<DST *>((SRC))
        switch (node->type) {
            case ENodeType::Compound:
                return visitCompound(TO(FCompoundNode, node));

            case ENodeType::Namespace:
                return visitNamespace(TO(FNamespaceNode, node));

            case ENodeType::Directive:
                return visitDirective(TO(FDirectiveNode, node));

            case ENodeType::VariableDeclare:
                return visitVariableDeclare(TO(FVariableDeclareNode, node));

            case ENodeType::FunctionDeclare:
                return visitFunctionDeclare(TO(FFunctionDeclareNode, node));

            case ENodeType::StructDeclare:
                return visitStructDeclare(TO(FStructDeclareNode, node));

            case ENodeType::ClassDeclare:
                return visitClassDeclare(TO(FClassDeclareNode, node));

            case ENodeType::EnumDeclare:
                return visitEnumDeclare(TO(FEnumDeclareNode, node));

            case ENodeType::Property:
                return visitProperty(TO(FPropertyNode, node));

            case ENodeType::EnumField:
                return visitEnumField(TO(FEnumFieldNode, node));

            case ENodeType::Literal:
                return visitLiteral(TO(FLiteralNode, node));

            default:
                return ReturnType{};
        }
#undef TO
    }

protected:
    virtual ReturnType visitCompound(FCompoundNode *node) {
        for (auto statement: node->statements) {
            visit(statement);
        }

        return ReturnType{};
    }

    virtual ReturnType visitNamespace(FNamespaceNode *node) {
        for (auto statement: node->statements) {
            visit(statement);
        }

        return ReturnType{};
    }

    virtual ReturnType visitDirective(FDirectiveNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitVariableDeclare(FVariableDeclareNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitFunctionDeclare(FFunctionDeclareNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitStructDeclare(FStructDeclareNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitClassDeclare(FClassDeclareNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitEnumDeclare(FEnumDeclareNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitProperty(FPropertyNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitEnumField(FEnumFieldNode *node) {
        return ReturnType{};
    }

    virtual ReturnType visitLiteral(FLiteralNode *node) {
        return ReturnType{};
    }
};