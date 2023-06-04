#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AICompositeNode.h"

class DLL_EXPORT AISelectorNode : public AICompositeNode {
public:
    EAIStatus tick() override;
};