#include "DX11Keyboard.h"
#include "Manager/InputManager.h"

DX11Keyboard::~DX11Keyboard() {
    Platform::UnRegisterPlatformEventHandler(this);
}

bool DX11Keyboard::initialize() {
    mDeviceType = EInputDeviceType::Keyboard;

    setupKeyNames();
    Platform::RegisterPlatformEventHandler(this);

    return true;
}

void DX11Keyboard::update() {
    InputDevice::update();
}

void DX11Keyboard::processKey(WPARAM wParam, LPARAM lParam) {
    auto scancode = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
    auto pressed = (HIWORD(lParam) & KF_UP) == 0;

    auto symbol = mSymbol[wParam];
    if (!symbol) {
        return;
    }

    EInputState newState;
    if (pressed) {
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

void DX11Keyboard::processUnicode(uint32_t codepoint) {
    UnicodeEvent event(codepoint);
    gInputManager().postUnicodeEvent(event);
}

bool DX11Keyboard::handleMessage(HWND hWnd, uint32_t uMsg, WPARAM wParam, LPARAM lParam, LRESULT *outResult) {
    switch (uMsg) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            processKey(wParam, lParam);
            *outResult = 0;
            return true;

        case WM_CHAR:
        case WM_SYSCHAR:
            if (wParam >= 0xd800 && wParam <= 0xdbff) {
                mHighSurrogate = wParam;
            } else {
                uint32_t codepoint = 0;
                if (wParam >= 0xdc00 && wParam <= 0xdfff) {
                    if (mHighSurrogate) {
                        codepoint += (mHighSurrogate - 0xd800) << 10;
                        codepoint += (char16_t) wParam - 0xdc00;
                        codepoint += 0x10000;
                    }
                } else {
                    codepoint = wParam;
                }

                mHighSurrogate = 0;
                processUnicode(codepoint);

                *outResult = 0;
            }
            return true;

        case WM_UNICHAR: {
            if (wParam == UNICODE_NOCHAR) {
                // WM_UNICHAR is not sent by Windows, but is sent by some
                // third-party input method engine
                // Returning TRUE here announces support for this message
                return TRUE;
            }

            processUnicode(wParam);

            *outResult = 0;
            return true;
        }
    }

    return false;
}

#define VK_NUM(X) (X - '0') + 48
#define VK_ALPHA(X) (X - 'a') + 65

void DX11Keyboard::setupKeyNames() {
    mSymbol[VK_NUM('0')] = mapSymbol(VK_NUM('0'), EKeyCode::Alpha0, TEXT("0"));
    mSymbol[VK_NUM('1')] = mapSymbol(VK_NUM('1'), EKeyCode::Alpha1, TEXT("1"));
    mSymbol[VK_NUM('2')] = mapSymbol(VK_NUM('2'), EKeyCode::Alpha2, TEXT("2"));
    mSymbol[VK_NUM('3')] = mapSymbol(VK_NUM('3'), EKeyCode::Alpha3, TEXT("3"));
    mSymbol[VK_NUM('4')] = mapSymbol(VK_NUM('4'), EKeyCode::Alpha4, TEXT("4"));
    mSymbol[VK_NUM('5')] = mapSymbol(VK_NUM('5'), EKeyCode::Alpha5, TEXT("5"));
    mSymbol[VK_NUM('6')] = mapSymbol(VK_NUM('6'), EKeyCode::Alpha6, TEXT("6"));
    mSymbol[VK_NUM('7')] = mapSymbol(VK_NUM('7'), EKeyCode::Alpha7, TEXT("7"));
    mSymbol[VK_NUM('8')] = mapSymbol(VK_NUM('8'), EKeyCode::Alpha8, TEXT("8"));
    mSymbol[VK_NUM('9')] = mapSymbol(VK_NUM('9'), EKeyCode::Alpha9, TEXT("9"));

    mSymbol[VK_ALPHA('a')] = mapSymbol(VK_ALPHA('a'), EKeyCode::A, TEXT("a"));
    mSymbol[VK_ALPHA('b')] = mapSymbol(VK_ALPHA('b'), EKeyCode::B, TEXT("b"));
    mSymbol[VK_ALPHA('c')] = mapSymbol(VK_ALPHA('c'), EKeyCode::C, TEXT("c"));
    mSymbol[VK_ALPHA('d')] = mapSymbol(VK_ALPHA('d'), EKeyCode::D, TEXT("d"));
    mSymbol[VK_ALPHA('e')] = mapSymbol(VK_ALPHA('e'), EKeyCode::E, TEXT("e"));
    mSymbol[VK_ALPHA('f')] = mapSymbol(VK_ALPHA('f'), EKeyCode::F, TEXT("f"));
    mSymbol[VK_ALPHA('g')] = mapSymbol(VK_ALPHA('g'), EKeyCode::G, TEXT("g"));
    mSymbol[VK_ALPHA('h')] = mapSymbol(VK_ALPHA('h'), EKeyCode::H, TEXT("h"));
    mSymbol[VK_ALPHA('i')] = mapSymbol(VK_ALPHA('i'), EKeyCode::I, TEXT("i"));
    mSymbol[VK_ALPHA('j')] = mapSymbol(VK_ALPHA('j'), EKeyCode::J, TEXT("j"));
    mSymbol[VK_ALPHA('k')] = mapSymbol(VK_ALPHA('k'), EKeyCode::K, TEXT("k"));
    mSymbol[VK_ALPHA('l')] = mapSymbol(VK_ALPHA('l'), EKeyCode::L, TEXT("l"));
    mSymbol[VK_ALPHA('m')] = mapSymbol(VK_ALPHA('m'), EKeyCode::M, TEXT("m"));
    mSymbol[VK_ALPHA('n')] = mapSymbol(VK_ALPHA('n'), EKeyCode::N, TEXT("n"));
    mSymbol[VK_ALPHA('o')] = mapSymbol(VK_ALPHA('o'), EKeyCode::O, TEXT("o"));
    mSymbol[VK_ALPHA('p')] = mapSymbol(VK_ALPHA('p'), EKeyCode::P, TEXT("p"));
    mSymbol[VK_ALPHA('q')] = mapSymbol(VK_ALPHA('q'), EKeyCode::Q, TEXT("q"));
    mSymbol[VK_ALPHA('r')] = mapSymbol(VK_ALPHA('r'), EKeyCode::R, TEXT("r"));
    mSymbol[VK_ALPHA('s')] = mapSymbol(VK_ALPHA('s'), EKeyCode::S, TEXT("s"));
    mSymbol[VK_ALPHA('t')] = mapSymbol(VK_ALPHA('t'), EKeyCode::T, TEXT("t"));
    mSymbol[VK_ALPHA('u')] = mapSymbol(VK_ALPHA('u'), EKeyCode::U, TEXT("u"));
    mSymbol[VK_ALPHA('v')] = mapSymbol(VK_ALPHA('v'), EKeyCode::V, TEXT("v"));
    mSymbol[VK_ALPHA('w')] = mapSymbol(VK_ALPHA('w'), EKeyCode::W, TEXT("w"));
    mSymbol[VK_ALPHA('x')] = mapSymbol(VK_ALPHA('x'), EKeyCode::X, TEXT("x"));
    mSymbol[VK_ALPHA('y')] = mapSymbol(VK_ALPHA('y'), EKeyCode::Y, TEXT("y"));
    mSymbol[VK_ALPHA('z')] = mapSymbol(VK_ALPHA('z'), EKeyCode::Z, TEXT("z"));

    mSymbol[VK_UP] = mapSymbol(VK_UP, EKeyCode::UpArrow, TEXT("UpArrow"));
    mSymbol[VK_DOWN] = mapSymbol(VK_DOWN, EKeyCode::DownArrow, TEXT("DownArrow"));
    mSymbol[VK_LEFT] = mapSymbol(VK_LEFT, EKeyCode::LeftArrow, TEXT("LeftArrow"));
    mSymbol[VK_RIGHT] = mapSymbol(VK_RIGHT, EKeyCode::RightArrow, TEXT("RightArrow"));
}
