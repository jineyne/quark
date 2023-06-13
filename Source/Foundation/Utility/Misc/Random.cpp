#include "Random.h"

Random::Random() : mGenerator(mDevice()) {}

uint32_t Random::get() {
    std::uniform_int_distribution<uint32_t> dis(0);
    return dis(mGenerator);
}

int Random::range(int min, int max) {
    assert(max > min);

    std::uniform_int_distribution<int> dis(min, max);
    return dis(mGenerator);
}

float Random::range(float min, float max) {
    assert(max > min);

    std::uniform_real_distribution<float> dis(min, max);
    return dis(mGenerator);
}

float Random::getUNorm() {
    return static_cast<float>(get() & 0x007FFFFF) / 8388607.0f;
}

float Random::getSNorm() {
    return getUNorm() * 2.0f - 1.0f;
}
