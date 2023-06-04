#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AICompositeNode.h"

class DLL_EXPORT AISequenceNode : public AICompositeNode {
public:
    EAIStatus tick() override;
};