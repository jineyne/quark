#include "RenderAPI.h"

DEFINE_LOG_CATEGORY(LogRenderAPI);

RenderWindow *RenderAPI::initialize(const RenderWindowDesc &desc) {
    initialize();

    auto renderWindow = RenderWindow::New(desc);
    initializeWithWindow(renderWindow);

    return renderWindow;
}

void RenderAPI::initialize() {

}

void RenderAPI::initializeWithWindow(RenderWindow *window) {

}

const RenderAPICapabilities &RenderAPI::getCapabilities(uint32_t deviceIdx) const {
    if(deviceIdx >= mDeviceCount) {
        LOG(LogRenderAPI, Warning, TEXT("Invalid device gIBO provided: %ld. Valid range is: [0, %ld)."), deviceIdx, mDeviceCount);
        return mCurrentCapabilities[0];
    }

    return mCurrentCapabilities[deviceIdx];
}


RenderAPI &gRenderAPI() {
    return RenderAPI::Instance();
}
