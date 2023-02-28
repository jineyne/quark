#include "Win32Window.h"

#include "Exception/Exception.h"

FWin32Window::FWin32Window(const FWin32WindowDesc &desc) : mDesc(desc) {
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = mDesc.proc;
    wc.hInstance = mDesc.instance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = TEXT("Win32Wnd");

    if (RegisterClassEx(&wc) == 0) {
        EXCEPT(LogWin32, InternalErrorException, TEXT("Failed to register window"));
    }

    COORD pos;
    pos.X = mDesc.left < 0 ? (GetSystemMetrics(SM_CXSCREEN) - mDesc.width) / 2 : mDesc.left;
    pos.Y = mDesc.top < 0 ? (GetSystemMetrics(SM_CYSCREEN) - mDesc.height) / 2 : mDesc.top;

    mWnd = CreateWindowEx(WS_EX_APPWINDOW, TEXT("Win32Wnd"), *mDesc.title,
                          WS_CLIPSIBLINGS | WS_CLIPCHILDREN, pos.X, pos.Y, mDesc.width,
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

    UnregisterClass(*mDesc.title, mDesc.instance);
    mDesc.instance = nullptr;
}

void FWin32Window::show() {
    ShowWindow(mWnd, SW_SHOW);
    UpdateWindow(mWnd);
}

void FWin32Window::hide() {
    ShowWindow(mWnd, SW_HIDE);
}

void FWin32Window::minimize() {
    ShowWindow(mWnd, SW_MINIMIZE);
}

void FWin32Window::maximize(bool maximized) {
    if (maximized) {
        ShowWindow(mWnd, SW_MAXIMIZE);
    } else {
        ShowWindow(mWnd, SW_RESTORE);
    }
}

void FWin32Window::resize(int32_t width, int32_t height) {
    RECT rect;
    GetWindowRect(mWnd, &rect);
    int x = rect.left;
    int y = rect.top;
    SetWindowPos(mWnd, NULL, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void FWin32Window::move(int32_t left, int32_t top) {
    RECT rect;
    GetWindowRect(mWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    SetWindowPos(mWnd, NULL, left, top, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void FWin32Window::setTitle(const FString &title) {
    SetWindowText(mWnd, *title);
}

HWND FWin32Window::getHandle() const {
    return mWnd;
}

int32_t FWin32Window::getLeft() const {
    RECT rect;
    GetWindowRect(mWnd, &rect);
    return rect.left;
}

int32_t FWin32Window::getTop() const {
    RECT rect;
    GetWindowRect(mWnd, &rect);
    return rect.top;
}

uint32_t FWin32Window::getWidth() const {
    RECT rect;
    GetWindowRect(mWnd, &rect);
    return rect.right - rect.left;
}

uint32_t FWin32Window::getHeight() const {
    RECT rect;
    GetWindowRect(mWnd, &rect);
    return rect.bottom - rect.top;
}
