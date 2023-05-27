#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include <chrono>

class DLL_EXPORT Timer {
private:
    std::chrono::high_resolution_clock mHRClock;
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;

public:
    Timer();

public:
    void reset();

    uint64_t getMilliseconds() const;
    uint64_t getMicroseconds() const;
    uint64_t getStartMilliseconds() const;
};
