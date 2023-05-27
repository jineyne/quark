#include "DX11InputManager.h"

void DX11InputManager::onStartUp() {
    mKeyboard = new DX11Keyboard();
    if (!addInputDevice(mKeyboard)) {
        EXCEPT(FDX11, InternalErrorException, TEXT("Unable to initialize keyboard"));
    }

    mMouse = new DX11Mouse();
    if (!addInputDevice(mMouse)) {
        EXCEPT(FDX11, InternalErrorException, TEXT("Unable to initialize mouse"));
    }
}
