#include "VideoModeInfo.h"

FVideoMode::FVideoMode(uint32_t width, uint32_t height, float refreshRate)
        : width(width), height(height), refreshRate(refreshRate) {}

bool FVideoMode::operator==(const FVideoMode &rhs) const {
    return width == rhs.width && height == rhs.height && refreshRate == rhs.refreshRate;
}

bool FVideoMode::operator!=(const FVideoMode &rhs) const {
    return !(*this == rhs);
}

FVideoOutputInfo::~FVideoOutputInfo() {
    for (auto it : mVideoModeList) {
        q_delete(it);
    }
    mVideoModeList.clear();
}

FVideoModeInfo::~FVideoModeInfo() {
    for (auto it : mVideoOutputList) {
        q_delete(it);
    }

    mVideoOutputList.clear();
}