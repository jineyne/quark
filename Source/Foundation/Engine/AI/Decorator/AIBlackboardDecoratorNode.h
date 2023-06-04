#pragma once

#include "Prerequisites.h"
#include "AI/AIDecoratorNode.h"
#include "AI/AIBlackboard.h"

class DLL_EXPORT AIBlackboardDecoratorNode : public AIDecoratorNode {
public:
    enum class EBlackboardDecoratorKeyQuery {
        IsSet,
        IsNotSet,
    };

private:
    EBlackboardDecoratorKeyQuery mKeyQuery;
    String mBlackboardKey;

public:
    AIBlackboardDecoratorNode(AINode *node);

public:
    bool conditionCheck() const override;

    EBlackboardDecoratorKeyQuery getKeyQuery() const;
    void setKeyQuery(EBlackboardDecoratorKeyQuery checkType);

    const String &getBlackboardKey() const;
    void setBlackboardKey(const String &blackboardKey);
};