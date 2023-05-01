#pragma once

#include "CorePrerequisites.h"
#include "InputType.h"

class DLL_EXPORT FInputDevice {
private:
    FString mDeviceName;

protected:
    EInputDeviceType mDeviceType;
    uint8_t mUniqueId;
    bool bIsEnabled;

    TMap<FString, FInputSymbol *> mNameToInfo;
    TMap<EKeyCode, FInputSymbol *> mCodeToInfo;
    TMap<uint32_t, FInputSymbol *> mDevSpecIdToInfo;

public:
    virtual ~FInputDevice();

public:
    virtual bool initialize();
    virtual void update();

    virtual bool inputState(const FString &name, EInputState state);

    void clearKeyState();

    void setUniqueId(uint8_t uniqueId);

    const TCHAR *getDeviceName() const { return *mDeviceName; }
    EInputDeviceType getDeviceType() const { return mDeviceType; }
    const TCHAR *getKeyName(EKeyCode code) const;

protected:
    FInputSymbol *getSymbolByName(FString name) const;
    FInputSymbol *getSymbolByDevSpecId(uint32_t devSpecId) const;

    FInputSymbol *mapSymbol(uint32_t id, EKeyCode code, FString name, FInputSymbol::EType type = FInputSymbol::EType::Button, uint32_t user = 0);
};