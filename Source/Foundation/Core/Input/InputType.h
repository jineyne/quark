#pragma once

#include "CorePrerequisites.h"

struct EModifierMask {
    enum Value {
        None       = 0,
        LCtrl      = (1 << 0),
        LShift     = (1 << 1),
        LAlt       = (1 << 2),
        LWin       = (1 << 3),
        RCtrl      = (1 << 4),
        RShift     = (1 << 5),
        RAlt       = (1 << 6),
        RWin       = (1 << 7),
        NumLock    = (1 << 8),
        CapsLock   = (1 << 9),
        ScrollLock = (1 << 10),

        Ctrl       = (LCtrl | RCtrl),
        Shift      = (LShift | RShift),
        Alt        = (LAlt | RAlt),
        Win        = (LWin | RWin),
        Modifiers  = (Ctrl | Shift | Alt | Win),
        LockKeys   = (CapsLock | NumLock | ScrollLock)
    };
};

const uint32_t KeyboardBase = 0;
const uint32_t MouseBase = 256;
const uint32_t TouchBase = 512;

enum class EKeyCode {
    Escape = KeyboardBase, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    Grave, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9, Alpha0, Minus, Equal, Backspace,
    Tab, Q, W, E, R, T, Y, U, I, O, P, LBracket, RBracket, Backslash,
    CapsLock, A, S, D, F, G, H, J, K, L, Semicolon, Apostrophe, Enter,
    LShift, Z, X, C, V, B, N, M, Comma, Period, Slash, RShift,
    LCtrl, LAlt, Space, RAlt, RCtrl,
    PrintScreen, ScreenLock, Pause,
    Insert, Home, PageUp, Delete, End, PageDown,
    UpArrow, LeftArrow, DownArrow, RightArrow,

    NumberLock, NumSlash, NumAsterisk, NumMinus,
    Num7, Num8, Num9, NumAdd,
    Num4, Num5, Num6,
    Num1, Num2, Num3, NumEnter,
    Num0, NumPeriod,
    KeyboardLast,

    Mouse1 = MouseBase,
    Mouse2,
    Mouse3,
    Mouse4,
    Mouse5,
    Mouse6,
    Mouse7,
    Mouse8,
    Mouse9,
    MouseWheelUp,
    MouseWheelDown,
    MouseX, MouseY, MouseZ,
    MouseLast,

    MouseLeft = Mouse1,
    MouseRight = Mouse2,
    MouseMiddle = Mouse3,

    Touch1 = TouchBase,
    TouchX, TouchY,
    TouchLast,

    Unknown,
};

enum class EInputDeviceType {
    Keyboard,
    Mouse,
    Touch,
    Unknown = 0xff,
};

enum class EInputState {
    None = 0,
    Released,
    Pressed,
    Repeat,
    Changed,
};

struct InputEvent {
    EInputDeviceType deviceType;
    EInputState state;
    String keyName;
    EKeyCode keyCode;
    int modifier;
    float value;
    struct InputSymbol *symbol;
    uint8_t deviceIndex;
    uint8_t deviceUniqueId;

    InputEvent()
            : deviceType(EInputDeviceType::Unknown), state(EInputState::None), keyName("")
            , keyCode(EKeyCode::Unknown), modifier(0), value(0), symbol(nullptr), deviceIndex(0), deviceUniqueId(0) {}
};

struct UnicodeEvent {
    const uint32_t data;

    UnicodeEvent(uint32_t data = 0) : data(data) {}
};

struct TouchEvent {
    EInputDeviceType deviceType;
    EInputState state;
    EKeyCode keyCode;
    uint8_t deviceIndex;
    uint32_t id;
    Vector2 value;
};

struct InputSymbol {
    enum EType {
        Button,
        Toggle,
        Axis,
        RawAxis,
    };

    const EKeyCode keyCode;
    const String keyName;
    const uint32_t id;
    EInputState state;
    EType type;
    EInputDeviceType deviceType;
    float value;
    uint32_t user;

    InputSymbol(uint32_t id, EKeyCode code, String name, EType type, uint32_t user = 0)
            : id(id), keyCode(code), keyName(std::move(name)), type(type), state(EInputState::None), user(user) {}

    void press(bool pressed) {
        if (pressed) {
            if (state == EInputState::Pressed) {
                state = EInputState::Repeat;
            } else {
                state = EInputState::Pressed;
            }
            value = 1.0f;
        } else {
            state = EInputState::Released;
            value = 0.0f;
        }
    }

    void change(float value) {
        state = EInputState::Changed;
        this->value = value;
    }

    void assignTo(InputEvent &event, int modifier) {
        event.symbol = this;
        event.deviceType = deviceType;
        event.modifier = modifier;
        event.state = state;
        event.value = value;
        event.keyName = keyName;
        event.keyCode = keyCode;
    }
};

struct IInputEventListener {
    virtual ~IInputEventListener() {}

    virtual bool onInputEvent(const InputEvent &event) = 0;
    virtual bool onUnicodeEvent(const UnicodeEvent &event) { return false; }

    virtual int getPriority() const { return 0; }
};

struct ITouchEventListener {
    virtual ~ITouchEventListener() {}

    virtual void onTouchEvent(const TouchEvent &event) = 0;

    virtual int getPriority() const { return 0; }
};