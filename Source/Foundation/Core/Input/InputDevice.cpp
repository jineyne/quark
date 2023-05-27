#include "InputDevice.h"
#include "Manager/InputManager.h"

InputDevice::~InputDevice() {
    while (mCodeToInfo.empty()) {
        auto pair = mCodeToInfo.begin();
        auto info = pair->value;
        mCodeToInfo.remove(pair->key);

        q_delete(info);
    }
}

bool InputDevice::initialize() {
    return false;
}

void InputDevice::update() {

}

bool InputDevice::inputState(const String &name, EInputState state) {
    auto symbol = getSymbolByName(name);
    return symbol && symbol->state == state;
}

void InputDevice::clearKeyState() {
    for (auto pair : mCodeToInfo) {
        auto symbol = pair.value;
        if (symbol && symbol->value > 0.0f) {
            InputEvent event;
            event.deviceType = mDeviceType;
            event.keyName = symbol->keyName;
            event.keyCode = symbol->keyCode;
            event.state = EInputState::Released;
            event.value = 0.0f;
            event.symbol = symbol;

            symbol->value = 0;
            symbol->state = EInputState::Released;

            gInputManager().postInputEvent(event);
        }
    }
}

void InputDevice::setUniqueId(uint8_t uniqueId) {
    mUniqueId = uniqueId;
}

const TCHAR *InputDevice::getKeyName(EKeyCode code) const {
    auto it = mCodeToInfo.find(code);
    if (it == nullptr) {
        return nullptr;
    }

    auto info = *it;
    assert(info != nullptr);

    return *(info->keyName);
}

InputSymbol *InputDevice::getSymbolByName(String name) const {
    auto it = mNameToInfo.find(name);
    if (it != nullptr) {
        return *it;
    } else {
        return nullptr;
    }
}

InputSymbol *InputDevice::getSymbolByDevSpecId(uint32_t devSpecId) const {
    auto it = mDevSpecIdToInfo.find(devSpecId);
    if (it != nullptr) {
        return *it;
    } else {
        return nullptr;
    }
}

InputSymbol *InputDevice::mapSymbol(uint32_t id, EKeyCode code, String name, InputSymbol::EType type,
                                    uint32_t user) {
    auto symbol = new InputSymbol(id, code, name, type);
    symbol->user = user;
    symbol->deviceType = mDeviceType;
    symbol->value = 0;

    mCodeToInfo[code] = symbol;
    mNameToInfo[name] = symbol;
    mDevSpecIdToInfo[id] = symbol;

    return symbol;
}
