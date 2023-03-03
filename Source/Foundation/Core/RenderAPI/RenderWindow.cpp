#include "RenderWindow.h"
#include "Manager/RenderWindowManager.h"

FRenderWindow::FRenderWindow(const FRenderWindowDesc &desc, uint32_t windowId, FRenderWindow *parent)
        : mDesc(desc), mWindowID(windowId) {
    mWidth = desc.videoMode.width;
    mHeight = desc.videoMode.height;
}

FRenderWindow *FRenderWindow::New(const FRenderWindowDesc &desc, FRenderWindow *parent) {
    return gRenderWindowManager().create(desc);
}

FRenderWindow::~FRenderWindow() {
    gRenderWindowManager().notifyWindowDestroyed(this);
}

void FRenderWindow::show() {
    bHidden = false;
}

void FRenderWindow::hide() {
    bHidden = true;
}

void FRenderWindow::minimize() {
    mWindowSizeState = EWindowSizeState::Minimize;
}

void FRenderWindow::maximize(bool maximized) {
    if (maximized) {
        mWindowSizeState = EWindowSizeState::Maximize;
    } else {
        mWindowSizeState = EWindowSizeState::Normal;
    }
}