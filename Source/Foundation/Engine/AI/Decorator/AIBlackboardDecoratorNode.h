#pragma once

#include "Prerequisites.h"
#include "AI/AIDecoratorNode.h"
#include "AI/AIBlackboard.h"
#include "AIBlackboardDecoratorNode.g.h"

QENUM()
enum class EBlackboardDecoratorKeyQuery {
    IsSet       QENTRY(),
    IsNotSet    QENTRY(),
};

QCLASS()
class DLL_EXPORT AIBlackboardDecoratorNode : public AIDecoratorNode {
public:
    GENERATED_BODY();

private:
    QPROPERTY()
    EBlackboardDecoratorKeyQuery mKeyQuery;

    QPROPERTY()
    String mBlackboardKey;

private:
    AIBlackboardDecoratorNode() = default;

public:
    AIBlackboardDecoratorNode(AINode *node);

public:
    bool conditionCheck() const override;

    EBlackboardDecoratorKeyQuery getKeyQuery() const;
    void setKeyQuery(EBlackboardDecoratorKeyQuery checkType);

    const String &getBlackboardKey() const;
    void setBlackboardKey(const String &blackboardKey);
};