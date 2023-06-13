#pragma once

#include "TestUtility.h"
#include "SaveData.g.h"

QSTRUCT()
struct SaveData {
    GENERATED_BODY();

    QPROPERTY()
    int level;

    QPROPERTY()
    int coin;
};
