#pragma once

#include "CorePrerequisites.h"
#include "InputType.h"

class DLL_EXPORT InputDevice {
private:
    String mDeviceName;

protected:
    EInputDeviceType mDeviceType;
    uint8_t mUniqueId;
    bool bIsEnabled;

    TMap<String, InputSymbol *> mNameToInfo;
    TMap<EKeyCode, InputSymbol *> mCodeToInfo;
    TMap<uint32_t, InputSymbol *> mDevSpecIdToInfo;

public:
    virtual ~InputDevice();

public:
    virtual bool initialize();
    virtual void update();

    virtual bool inputState(const String &name, EInputState state);

    void clearKeyState();

    void setUniqueId(uint8_t uniqueId);

    const TCHAR *getDeviceName() const { return *mDeviceName; }
    EInputDeviceType getDeviceType() const { return mDeviceType; }
    const TCHAR *getKeyName(EKeyCode code) const;

protected:
    InputSymbol *getSymbolByName(String name) const;
    InputSymbol *getSymbolByDevSpecId(uint32_t devSpecId) const;

    InputSymbol *mapSymbol(uint32_t id, EKeyCode code, String name, InputSymbol::EType type = InputSymbol::EType::Button, uint32_t user = 0);
};