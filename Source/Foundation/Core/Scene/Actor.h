#pragma once

#include "CorePrerequisites.h"
#include "Transform.h"

#define GENERATED_BODY()

QCLASS()
class FActor {
    GENERATED_BODY();
private:
    FTransform *mTransform;
    TArray<FActor> mAttachedActorList;
    FActor *mParent;

public:
    FActor();
    virtual ~FActor() = default;

};
