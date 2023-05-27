#pragma once

#include "TestCommonPrerequisites.h"
#include "Misc/Module.h"
#include "DynLibTestModule.g.h"

QCLASS(abstract)
class TestCommon_EXPORT FDynLibTestModule : public TModule<FDynLibTestModule> {
public:
    GENERATED_BODY();

public:
    QFUNCTION()
    virtual int getData();
};
