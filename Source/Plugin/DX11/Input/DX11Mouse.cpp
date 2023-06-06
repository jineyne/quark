#include "DX11Mouse.h"
#include "Manager/InputManager.h"

DX11Mouse::~DX11Mouse() {
    Platform::UnRegisterPlatformEventHandler(this);
}

#define MOUSE(EXP) static_cast<int>(EKeyCode::EXP) - MouseBase

bool DX11Mouse::initialize() {
    mDeviceType = EInputDeviceType::Mouse;

    Platform::RegisterPlatformEventHandler(this);

    mSymbol[MOUSE(Mouse1)] = mapSymbol(MOUSE(Mouse1), EKeyCode::Mouse1, "mouse1");
    mSymbol[MOUSE(Mouse2)] = mapSymbol(MOUSE(Mouse2), EKeyCode::Mouse2, "mouse2");
    mSymbol[MOUSE(Mouse3)] = mapSymbol(MOUSE(Mouse3), EKeyCode::Mouse3, "mouse3");
    mSymbol[MOUSE(Mouse4)] = mapSymbol(MOUSE(Mouse4), EKeyCode::Mouse4, "mouse4");
    mSymbol[MOUSE(Mouse5)] = mapSymbol(MOUSE(Mouse5), EKeyCode::Mouse5, "mouse5");
    mSymbol[MOUSE(Mouse6)] = mapSymbol(MOUSE(Mouse6), EKeyCode::Mouse6, "mouse6");
    mSymbol[MOUSE(Mouse7)] = mapSymbol(MOUSE(Mouse7), EKeyCode::Mouse7, "mouse7");
    mSymbol[MOUSE(Mouse8)] = mapSymbol(MOUSE(Mouse8), EKeyCode::Mouse8, "mouse8");

    mSymbol[MOUSE(MouseX)] = mapSymbol(MOUSE(MouseX), EKeyCode::MouseX, "mouse_x", InputSymbol::RawAxis);
    mSymbol[MOUSE(MouseY)] = mapSymbol(MOUSE(MouseY), EKeyCode::MouseY, "mouse_y", InputSymbol::RawAxis);
    mSymbol[MOUSE(MouseZ)] = mapSymbol(MOUSE(MouseZ), EKeyCode::MouseZ, "mouse_z", InputSymbol::RawAxis);

    mSymbol[MOUSE(MouseWheelDown)] = mapSymbol(MOUSE(MouseWheelDown), EKeyCode::MouseWheelDown, "mwheel_down");
    mSymbol[MOUSE(MouseWheelUp)] = mapSymbol(MOUSE(MouseWheelUp), EKeyCode::MouseWheelUp, "mwheel_up");

    return true;
}

void DX11Mouse::update() {
    InputDevice::update();

    mMouseWheel = 0;
}

bool DX11Mouse::handleMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam, LRESULT *outResult) {
    switch (uMsg) {
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
            processKey(uMsg, wParam, lParam);
            *outResult = 0;
            return true;

        case WM_MOUSEMOVE:
            processMotion(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            *outResult = 0;
            return true;

        case WM_MOUSEWHEEL:
            processScroll(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_WHEEL_DELTA_WPARAM(wParam));
            *outResult = 0;
            return true;
    }

    return false;
}

void DX11Mouse::processKey(uint32_t uMsg, WPARAM wParam, LPARAM lParam) {
    EKeyCode button;
    if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP) {
        button = EKeyCode::MouseLeft;
    } else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP) {
        button = EKeyCode::MouseRight;
    } else if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP) {
        button = EKeyCode::MouseMiddle;
    } else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) {
        button = EKeyCode::Mouse4;
    } else {
        button = EKeyCode::Mouse5;
    }

    auto symbol = getSymbolByDevSpecId(static_cast<int>(button) - MouseBase);
    if (!symbol) {
        return;
    }

    EInputState newState;
    if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_XBUTTONDOWN) {
        newState = EInputState::Pressed;
        symbol->value = 1.0f;
    } else {
        newState = EInputState::Released;
        symbol->value = 0.0f;
    }

    if (newState == symbol->state) {
        return;
    }

    symbol->state = newState;

    InputEvent event;
    symbol->assignTo(event, 0);

    gInputManager().postInputEvent(event);
}

void DX11Mouse::processMotion(float x, float y) {
    {
        auto symbol = mSymbol[MOUSE(MouseX)];
        if (symbol->value != x) {
            symbol->state = EInputState::Changed;
            symbol->value = x;
        }

        InputEvent event;
        symbol->assignTo(event, 0);
        gInputManager().postInputEvent(event);
    }

    {
        auto symbol = mSymbol[MOUSE(MouseY)];
        if (symbol->value != y) {
            symbol->state = EInputState::Changed;
            symbol->value = y;
        }

        InputEvent event;
        symbol->assignTo(event, 0);
        gInputManager().postInputEvent(event);
    }
}

void DX11Mouse::processScroll(float x, float y, float delta) {
    InputSymbol *symbol = delta > 0 ? mSymbol[MOUSE(MouseWheelUp)] : mSymbol[MOUSE(MouseWheelDown)];
    symbol->value = delta;
    symbol->state = EInputState::Changed;

    InputEvent event;
    symbol->assignTo(event, 0);
    gInputManager().postInputEvent(event);

    mMouseWheel += delta;
}
