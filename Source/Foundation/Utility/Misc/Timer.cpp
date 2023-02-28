#include "Timer.h"

FTimer::FTimer() {
    reset();
}

void FTimer::reset() {
    mStartTime = mHRClock.now();
}

uint64_t FTimer::getMilliseconds() const {
    auto now = mHRClock.now();
    std::chrono::duration<double> dur = now - mStartTime;
    return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
}

uint64_t FTimer::getMicroseconds() const {
    auto now = mHRClock.now();
    std::chrono::duration<double> dur = now - mStartTime;
    return std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
}

uint64_t FTimer::getStartMilliseconds() const {
    std::chrono::nanoseconds ns = mStartTime.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(ns).count();
}
