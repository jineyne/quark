#pragma once

#include "DX11Prerequisites.h"
#include "Manager/InputManager.h"
#include "DX11Keyboard.h"
#include "DX11Mouse.h"
#include "DX11InputManager.g.h"

QCLASS()
class DX11_EXPORT DX11InputManager : public InputManager {
    GENERATED_BODY()

private:
    DX11Keyboard *mKeyboard = nullptr;
    DX11Mouse *mMouse = nullptr;

protected:
    void onStartUp() override;
};