#include "Timer.h"

Timer::Timer() {
    reset();
}

void Timer::reset() {
    mStartTime = mHRClock.now();
}

uint64_t Timer::getMilliseconds() const {
    auto now = mHRClock.now();
    std::chrono::duration<double> dur = now - mStartTime;
    return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
}

uint64_t Timer::getMicroseconds() const {
    auto now = mHRClock.now();
    std::chrono::duration<double> dur = now - mStartTime;
    return std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
}

uint64_t Timer::getStartMilliseconds() const {
    std::chrono::nanoseconds ns = mStartTime.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(ns).count();
}
