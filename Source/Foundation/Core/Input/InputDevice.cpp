#include "InputDevice.h"
#include "InputManager.h"

FInputDevice::~FInputDevice() {
    while (mCodeToInfo.empty()) {
        auto pair = mCodeToInfo.begin();
        auto info = pair->second;
        mCodeToInfo.remove(pair->first);

        q_delete(info);
    }
}

bool FInputDevice::initialize() {
    return false;
}

void FInputDevice::update() {

}

bool FInputDevice::inputState(const FString &name, EInputState state) {
    auto symbol = getSymbolByName(name);
    return symbol && symbol->state == state;
}

void FInputDevice::clearKeyState() {
    for (auto pair : mCodeToInfo) {
        auto symbol = pair.second;
        if (symbol && symbol->value > 0.0f) {
            FInputEvent event;
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

void FInputDevice::setUniqueId(uint8_t uniqueId) {
    mUniqueId = uniqueId;
}

const TCHAR *FInputDevice::getKeyName(EKeyCode code) const {
    auto it = mCodeToInfo.find(code);
    if (it == nullptr) {
        return nullptr;
    }

    auto info = *it;
    assert(info != nullptr);

    return *(info->keyName);
}

FInputSymbol *FInputDevice::getSymbolByName(FString name) const {
    auto it = mNameToInfo.find(name);
    if (it != nullptr) {
        return *it;
    } else {
        return nullptr;
    }
}

FInputSymbol *FInputDevice::getSymbolByDevSpecId(uint32_t devSpecId) const {
    auto it = mDevSpecIdToInfo.find(devSpecId);
    if (it != nullptr) {
        return *it;
    } else {
        return nullptr;
    }
}

FInputSymbol *FInputDevice::mapSymbol(uint32_t id, EKeyCode code, FString name, FInputSymbol::EType type,
                                      uint32_t user) {
    auto symbol = new FInputSymbol(id, code, name, type);
    symbol->user = user;
    symbol->deviceType = mDeviceType;
    symbol->value = 0;

    mCodeToInfo[code] = symbol;
    mNameToInfo[name] = symbol;
    mDevSpecIdToInfo[id] = symbol;

    return symbol;
}
