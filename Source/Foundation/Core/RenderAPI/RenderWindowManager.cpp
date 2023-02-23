#include "RenderWindowManager.h"
#include "CoreApplication.h"

FRenderWindow *FRenderWindowManager::create(const FRenderWindowDesc &desc, FRenderWindow *parent) {
    uint32_t id = mNextWindowID++;
    auto window = createInternal(desc, id, parent);

    mWindowMap.add(id, window);
    return window;
}

void FRenderWindowManager::update() {
    TArray<FRenderWindow *> closeRequestedWindows;

    std::swap(mCloseRequestedWindowList, closeRequestedWindows);

    const auto primaryWindow = gCoreApplication().getPrimaryWindow();
    for (auto &&it : closeRequestedWindows) {
        if (it == primaryWindow) {
            gCoreApplication().quitRequest();
            continue;
        }
    }
}

FRenderWindow *FRenderWindowManager::getWindow(uint32_t id) {
    auto it = mWindowMap.find(id);
    if (it == nullptr) {
        return nullptr;
    }

    return *it;
}

void FRenderWindowManager::notifyWindowDestroyed(FRenderWindow *window) {
    mWindowMap.remove(window->getWindowID());
}

void FRenderWindowManager::notifyCloseRequested(FRenderWindow *window) {
    auto it = std::find(mCloseRequestedWindowList.begin(), mCloseRequestedWindowList.end(), window);
    if (it != mCloseRequestedWindowList.end()) {
        return;
    }

    mCloseRequestedWindowList.add(window);
}

FRenderWindowManager &gRenderWindowManager() {
    return FRenderWindowManager::Instance();
}
