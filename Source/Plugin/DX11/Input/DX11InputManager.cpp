#include "DX11InputManager.h"

void FDX11InputManager::onStartUp() {
    mKeyboard = new FDX11Keyboard();
    if (!addInputDevice(mKeyboard)) {
        EXCEPT(FDX11, InternalErrorException, TEXT("Unable to initialize keyboard"));
    }

    mMouse = new FDX11Mouse();
    if (!addInputDevice(mMouse)) {
        EXCEPT(FDX11, InternalErrorException, TEXT("Unable to initialize mouse"));
    }
}
