#include "VideoModeInfo.h"

VideoMode::VideoMode(uint32_t width, uint32_t height, float refreshRate)
        : width(width), height(height), refreshRate(refreshRate) {}

bool VideoMode::operator==(const VideoMode &rhs) const {
    return width == rhs.width && height == rhs.height && refreshRate == rhs.refreshRate;
}

bool VideoMode::operator!=(const VideoMode &rhs) const {
    return !(*this == rhs);
}

VideoOutputInfo::~VideoOutputInfo() {
    for (auto it : mVideoModeList) {
        q_delete(it);
    }
    mVideoModeList.clear();
}

VideoModeInfo::~VideoModeInfo() {
    for (auto it : mVideoOutputList) {
        q_delete(it);
    }

    mVideoOutputList.clear();
}