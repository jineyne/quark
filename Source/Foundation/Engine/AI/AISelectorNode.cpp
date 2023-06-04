#include "AISelectorNode.h"

EAIStatus AISelectorNode::tick() {
    for (auto child : mChildren) {
        auto status = child->tick();
        if (status != EAIStatus::Failure) {
            return status;
        }
    }

    return EAIStatus::Failure;
}
