#pragma once

#include "Prerequisites.h"
#include "Resource/Resource.h"
#include "AICompositeNode.h"
#include "AISelectorNode.g.h"

QCLASS()
class DLL_EXPORT AISelectorNode : public AICompositeNode {
    GENERATED_BODY();

public:
    EAIStatus tick() override;
};