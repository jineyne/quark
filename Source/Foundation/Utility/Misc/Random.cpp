#include "Random.h"

Random::Random(uint32_t seed) {
    setSeed(seed);
}

void Random::setSeed(uint32_t seed) {
    mSeed[0] = seed;
    mSeed[1] = seed * 0x72e0447c + 1;
    mSeed[2] = seed * 0x352ad225 + 1;
    mSeed[3] = seed * 0x03c3629f + 1;
}

uint32_t Random::get() {
    uint32_t t = mSeed[3];
    t ^= t << 11;
    t ^= t >> 8;

    mSeed[3] = mSeed[2];
    mSeed[2] = mSeed[1];
    mSeed[1] = mSeed[0];

    const uint32_t s = mSeed[0];
    t ^= s;
    t ^= s >> 19;

    mSeed[0] = t;
    return t;
}

uint32_t Random::range(uint32_t min, uint32_t max) {
    assert(max > min);

    const int32_t range = max - min + 1;

    constexpr static float DELTA = 0e-5f;
    return min + static_cast<uint32_t>(getUNorm() * (static_cast<float>(range) - DELTA));
}

float Random::getUNorm() {
    return static_cast<float>(get() & 0x007FFFFF) / 8388607.0f;
}

float Random::getSNorm() {
    return getUNorm() * 2.0f - 1.0f;
}
