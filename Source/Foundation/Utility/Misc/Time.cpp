#include "Time.h"

const double Time::MICROSEC_TO_SEC = 1.0 / 1000000.0;

Time::Time() {

}

void Time::update() {
    uint64_t currentFrameTime = mTimer->getMicroseconds();

    if (!mIsFirstFrame) {
        mDeltaTime = ((float)((currentFrameTime - mLastFrameTime) * MICROSEC_TO_SEC)) * mTimeScale;
    } else {
        mDeltaTime = 0;
        mIsFirstFrame = false;
    }

    mTimeSinceStart = (static_cast<float>(currentFrameTime) / 1000.0f / 1000.0f);
    mLastFrameTime = currentFrameTime;
}

void Time::setTimeScale(float scale) {
    mTimeScale = scale;
}

void Time::advanceFixedUpdate(uint64_t step) {
    mLastFixedUpdateTime += step;
}

uint64_t Time::getTimePrecise() const {
    return mTimer->getMicroseconds();
}

uint32_t Time::getFixedUpdateStep(uint64_t &step) {
    const uint64_t currentTime = getTimePrecise();

    // Skip fixed updateData first frame (Time delta is zero, and no input received yet)
    if (mFirstFixedFrame) {
        mLastFixedUpdateTime = currentTime;
        mFirstFixedFrame = false;
    }

    const int64_t nextFrameTime = mLastFixedUpdateTime + mFixedStep;
    if (nextFrameTime <= currentTime) {
        const int64_t simulationAmount = (int64_t)std::max(currentTime - mLastFixedUpdateTime, mFixedStep); // At least one step
        auto numIterations = static_cast<uint32_t>(Math::DivideAndRoundUp(simulationAmount,
                                                                          static_cast<int64_t>(mFixedStep)));

        auto stepus = static_cast<int64_t>(mFixedStep);
        if (numIterations > mNumRemainingFixedUpdates) {
            stepus = Math::DivideAndRoundUp(simulationAmount, (int64_t)mNumRemainingFixedUpdates);
            numIterations = static_cast<uint32_t>(Math::DivideAndRoundUp(simulationAmount,
                                                                         static_cast<int64_t>(stepus)));
        }

        assert(numIterations <= mNumRemainingFixedUpdates);

        mNumRemainingFixedUpdates -= numIterations;
        mNumRemainingFixedUpdates = std::min(MAX_ACCUM_FIXED_UPDATES,
                                             mNumRemainingFixedUpdates +
                                             NEW_FIXED_UPDATES_PER_FRAME);

        step = stepus;
        return numIterations;
    }

    step = 0;
    return 0;
}

float Time::getFixedDeltaTime() const {
    return mFixedDeltaTime;
}

void Time::setFixedDeltaTime(float fixedDeltaTime) {
    mFixedDeltaTime = fixedDeltaTime;
}

void Time::onStartUp() {
    mTimer = q_new<Timer>();
    mAppStartTime = mTimer->getStartMilliseconds();
}

void Time::onShutDown() {
    q_delete(mTimer);
}

Time &gTime() {
    return Time::Instance();
}