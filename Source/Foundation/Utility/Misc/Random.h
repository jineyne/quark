#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include <random>

class Random {
private:
    std::random_device mDevice;
    std::mt19937 mGenerator;

public:
    Random();

public:
    uint32_t get();

    int range(int min, int max);
    float range(float min, float max);

    float getUNorm();
    float getSNorm();
};