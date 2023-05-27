#include "RenderWindowManager.h"
#include "CoreApplication.h"

RenderWindow *RenderWindowManager::create(const RenderWindowDesc &desc, RenderWindow *parent) {
    uint32_t id = mNextWindowID++;
    auto window = createInternal(desc, id, parent);

    mWindowMap.add(id, window);
    return window;
}

void RenderWindowManager::update() {
    TArray<RenderWindow *> closeRequestedWindows;

    std::swap(mCloseRequestedWindowList, closeRequestedWindows);

    const auto primaryWindow = gCoreApplication().getPrimaryWindow();
    for (auto &&it : closeRequestedWindows) {
        if (it == primaryWindow) {
            gCoreApplication().quitRequest();
            continue;
        }
    }
}

RenderWindow *RenderWindowManager::getWindow(uint32_t id) {
    auto it = mWindowMap.find(id);
    if (it == nullptr) {
        return nullptr;
    }

    return *it;
}

void RenderWindowManager::notifyWindowDestroyed(RenderWindow *window) {
    mWindowMap.remove(window->getWindowID());
}

void RenderWindowManager::notifyCloseRequested(RenderWindow *window) {
    auto it = std::find(mCloseRequestedWindowList.begin(), mCloseRequestedWindowList.end(), window);
    if (it != mCloseRequestedWindowList.end()) {
        return;
    }

    mCloseRequestedWindowList.add(window);
}

RenderWindowManager &gRenderWindowManager() {
    return RenderWindowManager::Instance();
}
