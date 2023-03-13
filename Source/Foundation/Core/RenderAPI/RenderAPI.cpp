#include "RenderAPI.h"

DEFINE_LOG_CATEGORY(FLogRenderAPI);

FRenderWindow *FRenderAPI::initialize(const FRenderWindowDesc &desc) {
    initialize();

    auto renderWindow = FRenderWindow::New(desc);
    initializeWithWindow(renderWindow);

    return renderWindow;
}

void FRenderAPI::initialize() {

}

void FRenderAPI::initializeWithWindow(FRenderWindow *window) {

}

const FRenderAPICapabilities &FRenderAPI::getCapabilities(uint32_t deviceIdx) const {
    if(deviceIdx >= mDeviceCount) {
        LOG(FLogRenderAPI, Warning, TEXT("Invalid device gIBO provided: %ld. Valid range is: [0, %ld)."), deviceIdx, mDeviceCount);
        return mCurrentCapabilities[0];
    }

    return mCurrentCapabilities[deviceIdx];
}


FRenderAPI &gRenderAPI() {
    return FRenderAPI::Instance();
}
