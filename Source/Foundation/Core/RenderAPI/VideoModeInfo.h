#pragma once

#include "CorePrerequisites.h"

struct DLL_EXPORT VideoMode {
public:
    uint32_t width = 1280;

    uint32_t height = 720;

    float refreshRate = 60.f;

    bool isCustom = true;

public:
    VideoMode() = default;
    VideoMode(uint32_t width, uint32_t height, float refreshRate = 60.f);

    virtual ~VideoMode() = default;

public:
    bool operator==(const VideoMode &rhs) const;
    bool operator!=(const VideoMode &rhs) const;
};

class DLL_EXPORT VideoOutputInfo {
protected:
    String mName;
    TArray<VideoMode *> mVideoModeList;
    VideoMode *mDesktopVideoMode = nullptr;

public:
    VideoOutputInfo() = default;

    virtual ~VideoOutputInfo();

public:
    const auto &getName() const { return mName; }
    uint32_t getVideoModeSize() const { return static_cast<uint32_t>(mVideoModeList.length()); }
    auto getVideoMode(uint32_t size) const { return mVideoModeList[size]; }
    auto getDesktopVideoMode() const { return mDesktopVideoMode; }
};

class DLL_EXPORT VideoModeInfo {
protected:
    TArray<VideoOutputInfo *> mVideoOutputList;

public:
    VideoModeInfo() = default;
    virtual ~VideoModeInfo();

public:
    auto getOutputSize() const { return static_cast<uint32_t>(mVideoOutputList.length()); }
    VideoOutputInfo *getOutputInfo(uint32_t idx) const { return mVideoOutputList[idx]; }
};