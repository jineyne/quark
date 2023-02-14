#include "Win32Window.h"

#include "Exception/Exception.h"

FWin32Window::FWin32Window(const FWin32WindowDesc &desc) {
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = mDesc.proc;
    wc.hInstance = mDesc.instance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "Win32Wnd";

    if (RegisterClassEx(&wc) == 0) {
        EXCEPT(LogWin32, InternalErrorException, TEXT("Failed to register window"));
    }

    COORD pos;
    pos.X = mDesc.left < 0 ? (GetSystemMetrics(SM_CXSCREEN) - mDesc.width) / 2 : mDesc.left;
    pos.Y = mDesc.top < 0 ? (GetSystemMetrics(SM_CYSCREEN) - mDesc.height) / 2 : mDesc.top;

    mWnd = CreateWindowEx(WS_EX_APPWINDOW, "Win32Wnd", TCHAR_TO_ANSI(*mDesc.title),
                          WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, pos.X, pos.Y, mDesc.width,
                          mDesc.height, NULL, NULL, mDesc.instance, this);
    if (mWnd == nullptr) {
        EXCEPT(LogWin32, InternalErrorException, TEXT("Failed to create window"));
    }

    ShowWindow(mWnd, SW_SHOW);
    SetForegroundWindow(mWnd);
    SetFocus(mWnd);
}

FWin32Window::~FWin32Window() {
    DestroyWindow(mWnd);
    mWnd = nullptr;

    UnregisterClass(TCHAR_TO_ANSI(*mDesc.title), mDesc.instance);
    mDesc.instance = nullptr;
}

HWND FWin32Window::getHandle() const {
    return mWnd;
}

int32_t FWin32Window::getLeft() const {
    return mDesc.left;
}

int32_t FWin32Window::getTop() const {
    return mDesc.top;
}

uint32_t FWin32Window::getWidth() const {
    return mDesc.width;
}

uint32_t FWin32Window::getHeight() const {
    return mDesc.height;
}
