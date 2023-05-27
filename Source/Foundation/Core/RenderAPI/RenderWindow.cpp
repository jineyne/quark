#include "RenderWindow.h"
#include "Manager/RenderWindowManager.h"

RenderWindow::RenderWindow(const RenderWindowDesc &desc, uint32_t windowId, RenderWindow *parent)
        : mDesc(desc), mWindowID(windowId) {
    mWidth = desc.videoMode.width;
    mHeight = desc.videoMode.height;
}

RenderWindow *RenderWindow::New(const RenderWindowDesc &desc, RenderWindow *parent) {
    return gRenderWindowManager().create(desc);
}

RenderWindow::~RenderWindow() {
    gRenderWindowManager().notifyWindowDestroyed(this);
}

void RenderWindow::show() {
    bHidden = false;
}

void RenderWindow::hide() {
    bHidden = true;
}

void RenderWindow::minimize() {
    mWindowSizeState = EWindowSizeState::Minimize;
}

void RenderWindow::maximize(bool maximized) {
    if (maximized) {
        mWindowSizeState = EWindowSizeState::Maximize;
    } else {
        mWindowSizeState = EWindowSizeState::Normal;
    }
}