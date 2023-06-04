#include "AISequenceNode.h"

EAIStatus AISequenceNode::tick() {
    for (auto child : mChildren) {
        auto status = child->tick();
        if (status != EAIStatus::Success) {
            return status;
        }
    }

    return EAIStatus::Success;
}
