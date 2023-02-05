#pragma once

#include <gtest/gtest.h>
#include <Misc/Module.h>

#include "ModuleTest.g.h"

QCLASS()
class FTestModule : public TModule<FTestModule> {
public:
    GENERATED_BODY();

protected:
    void onStartUp() override;
    void onShutDown() override;
};