#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Exception/Exception.h"
#include "Reflection/Reflection.h"
#include "Timer.h"
#include "Module.h"
#include "Time.g.h"

QCLASS()
class DLL_EXPORT FTime : public TModule<FTime> {
    GENERATED_BODY()

private:
    static const double MICROSEC_TO_SEC;
    static constexpr uint32_t MAX_ACCUM_FIXED_UPDATES = 200;
    static constexpr uint32_t NEW_FIXED_UPDATES_PER_FRAME = 4;

    FTimer *mTimer;
    float mDeltaTime = 0.0f;
    float mTimeSinceStart = 0.0f;

    uint64_t mAppStartTime = 0;
    uint64_t mLastFrameTime = 0;

    bool mIsFirstFrame = true;

    // fixed update
    uint64_t mFixedStep = 16666; // 60 times a second in microseconds
    uint64_t mLastFixedUpdateTime = 0;
    bool mFirstFixedFrame = true;
    uint32_t mNumRemainingFixedUpdates = MAX_ACCUM_FIXED_UPDATES;

    float mTimeScale = 1.f;

public:
    FTime();

public:
    float getTime() const { return mTimeSinceStart; }

    float getDeltaTime() const { return mDeltaTime; }

    void update();

    void setTimeScale(float scale);

    void advanceFixedUpdate(uint64_t step);

    uint64_t getTimePrecise() const;

    uint32_t getFixedUpdateStep(uint64_t &step);

    float getTimeScale() const { return mTimeScale; }

protected:
    void onStartUp() override;

    void onShutDown() override;
};

DLL_EXPORT FTime &gTime();
