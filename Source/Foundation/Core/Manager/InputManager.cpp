#include "InputManager.h"

bool FInputManager::inputState(const FString &name, EInputState state) {
    for (auto device : mInputDeviceList) {
        if (device->inputState(name, state)) return true;
    }

    return false;
}

void FInputManager::addEventListener(IInputEventListener *listener) {
    mInputEventListenerList.add(listener);
}

void FInputManager::addEventListener(ITouchEventListener *listener) {
    mTouchEventListenerList.add(listener);
}

void FInputManager::removeEventListener(IInputEventListener *listener) {
    mInputEventListenerList.remove(listener);
}   

void FInputManager::removeEventListener(ITouchEventListener *listener) {
    mTouchEventListenerList.remove(listener);
}

void FInputManager::postInputEvent(const FInputEvent &event, bool force) {
    if (event.keyCode == EKeyCode::Unknown) {
        return;
    }

    if (!sendEventToListeners(event)) {
        return;
    }
}

void FInputManager::postUnicodeEvent(const FUnicodeEvent &event, bool force) {
    if (!sendEventToListeners(event)) {
        return;
    }
}

void FInputManager::postTouchEvent(const FTouchEvent &event, bool force) {
    for (auto listener : mTouchEventListenerList) {
        listener->onTouchEvent(event);
    }
}

void FInputManager::update() {
    for (auto device : mInputDeviceList) {
        device->update();
    }
}

void FInputManager::clearKeyState() {
    for (auto device : mInputDeviceList) {
        device->clearKeyState();
    }
}

bool FInputManager::addInputDevice(FInputDevice *inputDevice) {
    if (inputDevice) {
        if (inputDevice->initialize()) {
            static uint8_t uniqueId = 0;
            inputDevice->setUniqueId(uniqueId++);
            mInputDeviceList.add(inputDevice);
            return true;
        }
    }

    return false;
}

bool FInputManager::removeInputDevice(FInputDevice *inputDevice) {
    if (inputDevice) {
        for (auto it = mInputDeviceList.begin(); it != mInputDeviceList.end(); ++it) {
            if (*it == inputDevice) {
                mInputDeviceList.removeIt(it);
                delete inputDevice;
                return true;
            }
        }
    }

    return false;
}

void FInputManager::onShutDown() {
    while (!mInputDeviceList.empty()) {
        auto it = mInputDeviceList.begin();
        auto device = *it;
        device->clearKeyState();
        mInputDeviceList.removeIt(it);

        delete device;
    }
}

bool FInputManager::sendEventToListeners(const FInputEvent &event) {
    for (auto listener : mInputEventListenerList) {
        if (listener->onInputEvent(event)) {
            return true;
        }
    }

    return false;
}

bool FInputManager::sendEventToListeners(const FUnicodeEvent &event) {
    for (auto listener : mInputEventListenerList) {
        if (listener->onUnicodeEvent(event)) {
            return true;
        }
    }

    return false;
}

FInputManager &gInputManager() {
    return FInputManager::Instance();
}
