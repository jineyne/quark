#include "InputManager.h"

bool InputManager::inputState(const String &name, EInputState state) {
    for (auto device : mInputDeviceList) {
        if (device->inputState(name, state)) return true;
    }

    return false;
}

void InputManager::addEventListener(IInputEventListener *listener) {
    mInputEventListenerList.add(listener);
}

void InputManager::addEventListener(ITouchEventListener *listener) {
    mTouchEventListenerList.add(listener);
}

void InputManager::removeEventListener(IInputEventListener *listener) {
    mInputEventListenerList.remove(listener);
}   

void InputManager::removeEventListener(ITouchEventListener *listener) {
    mTouchEventListenerList.remove(listener);
}

void InputManager::postInputEvent(const InputEvent &event, bool force) {
    if (event.keyCode == EKeyCode::Unknown) {
        return;
    }

    if (!sendEventToListeners(event)) {
        return;
    }
}

void InputManager::postUnicodeEvent(const UnicodeEvent &event, bool force) {
    if (!sendEventToListeners(event)) {
        return;
    }
}

void InputManager::postTouchEvent(const TouchEvent &event, bool force) {
    for (auto listener : mTouchEventListenerList) {
        listener->onTouchEvent(event);
    }
}

void InputManager::update() {
    for (auto device : mInputDeviceList) {
        device->updateData();
    }
}

void InputManager::clearKeyState() {
    for (auto device : mInputDeviceList) {
        device->clearKeyState();
    }
}

bool InputManager::addInputDevice(InputDevice *inputDevice) {
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

bool InputManager::removeInputDevice(InputDevice *inputDevice) {
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

void InputManager::onShutDown() {
    while (!mInputDeviceList.empty()) {
        auto it = mInputDeviceList.begin();
        auto device = *it;
        device->clearKeyState();
        mInputDeviceList.removeIt(it);

        delete device;
    }
}

bool InputManager::sendEventToListeners(const InputEvent &event) {
    for (auto listener : mInputEventListenerList) {
        if (listener->onInputEvent(event)) {
            return true;
        }
    }

    return false;
}

bool InputManager::sendEventToListeners(const UnicodeEvent &event) {
    for (auto listener : mInputEventListenerList) {
        if (listener->onUnicodeEvent(event)) {
            return true;
        }
    }

    return false;
}

InputManager &gInputManager() {
    return InputManager::Instance();
}
