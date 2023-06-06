#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

class Random {
private:
    uint32_t mSeed[4];

public:
    Random(uint32_t seed = static_cast<uint32_t>(std::time(nullptr)));

public:
    void setSeed(uint32_t seed);

    uint32_t get();
    uint32_t range(uint32_t min, uint32_t max);

    float getUNorm();
    float getSNorm();
};