#pragma once

#include "UtilityCore.h"
#include "Misc/Module.h"
#include "DynLibTestModule.h"
#include "MyDynLibTestModule.g.h"

QCLASS()
class FMyDynLibTestModule : public FDynLibTestModule {
public:
    GENERATED_BODY();

public:
    int getData() override;
};