#pragma once

#include "CorePrerequisites.h"

struct DLL_EXPORT FVideoMode {
public:
    uint32_t width = 1280;

    uint32_t height = 720;

    float refreshRate = 60.f;

    bool isCustom = true;

public:
    FVideoMode() = default;
    FVideoMode(uint32_t width, uint32_t height, float refreshRate = 60.f);

    virtual ~FVideoMode() = default;

public:
    bool operator==(const FVideoMode &rhs) const;
    bool operator!=(const FVideoMode &rhs) const;
};

class DLL_EXPORT FVideoOutputInfo {
protected:
    FString mName;
    TArray<FVideoMode *> mVideoModeList;
    FVideoMode *mDesktopVideoMode = nullptr;

public:
    FVideoOutputInfo() = default;

    virtual ~FVideoOutputInfo();

public:
    const auto &getName() const { return mName; }
    uint32_t getVideoModeSize() const { return static_cast<uint32_t>(mVideoModeList.length()); }
    auto getVideoMode(uint32_t size) const { return mVideoModeList[size]; }
    auto getDesktopVideoMode() const { return mDesktopVideoMode; }
};

class DLL_EXPORT FVideoModeInfo {
protected:
    TArray<FVideoOutputInfo *> mVideoOutputList;

public:
    FVideoModeInfo() = default;
    virtual ~FVideoModeInfo();

public:
    auto getOutputSize() const { return static_cast<uint32_t>(mVideoOutputList.length()); }
    FVideoOutputInfo *getOutputInfo(uint32_t idx) const { return mVideoOutputList[idx]; }
};