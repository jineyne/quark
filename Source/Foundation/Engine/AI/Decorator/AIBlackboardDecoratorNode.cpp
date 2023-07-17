#include "AIBlackboardDecoratorNode.h"

AIBlackboardDecoratorNode::AIBlackboardDecoratorNode(AINode *node) : AIDecoratorNode(node) {}

bool AIBlackboardDecoratorNode::conditionCheck() const {
    if (mBlackboardKey.empty()) {
        return mKeyQuery == EBlackboardDecoratorKeyQuery::IsNotSet;
    }

    auto set = getBlackboard()->isSet(mBlackboardKey);
    return (set && mKeyQuery == EBlackboardDecoratorKeyQuery::IsSet) || (!set && mKeyQuery == EBlackboardDecoratorKeyQuery::IsNotSet);
}

EBlackboardDecoratorKeyQuery AIBlackboardDecoratorNode::getKeyQuery() const {
    return mKeyQuery;
}

void AIBlackboardDecoratorNode::setKeyQuery(EBlackboardDecoratorKeyQuery checkType) {
    mKeyQuery = checkType;
}

const String &AIBlackboardDecoratorNode::getBlackboardKey() const {
    return mBlackboardKey;
}

void AIBlackboardDecoratorNode::setBlackboardKey(const String &blackboardKey) {
    mBlackboardKey = blackboardKey;
}
