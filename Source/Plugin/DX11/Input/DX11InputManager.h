#pragma once

#include "DX11Prerequisites.h"
#include "Input/InputManager.h"
#include "DX11Keyboard.h"
#include "DX11Mouse.h"
#include "DX11InputManager.g.h"

QCLASS()
class DX11_EXPORT FDX11InputManager : public FInputManager {
    GENERATED_BODY()

private:
    FDX11Keyboard *mKeyboard = nullptr;
    FDX11Mouse *mMouse = nullptr;

protected:
    void onStartUp() override;
};