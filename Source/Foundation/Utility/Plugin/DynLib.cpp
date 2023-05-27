#if PLATFORM == PLATFORM_WIN32
#   define WIN32_LEAN_AND_MEAN
#   if !defined(NOMINMAX) && defined(_MSC_VER)
#       define NOMINMAX
#   endif
#   define _TCHAR_DEFINED
#   include <windows.h>
#   undef TEXT
#endif

#include "DynLib.h"

#include "Exception/Exception.h"

DEFINE_LOG_CATEGORY(LogDynLib);

DynLib::DynLib(const String &name) : mName(name) {
    load();
}

DynLib::~DynLib() {
    unload();
}

void DynLib::load() {

#if PLATFORM != PLATFORM_ANDROID
    if (mHandle) {
        return;
    }

    mHandle = (DYNLIB_HANDLE) DYNLIB_LOAD(*mName);

    if (!mHandle) {
        EXCEPT(LogDynLib, InternalErrorException, TEXT("Could not load dynamic library %ls. System Error: %ls"), *mName, *getError());
    }

    LOG(LogDynLib, Debug, TEXT("Load dynamic library: %s"), *mName);

#endif
}

void DynLib::unload() {
#if PLATFORM != PLATFORM_ANDROID
    if (!mHandle) {
        return;
    }

    if (DYNLIB_UNLOAD(mHandle)) {
        EXCEPT(LogDynLib, InternalErrorException, TEXT("Could not unload dynamic library %ls. System Error: %ls"), *mName, *getError());
    }

    LOG(LogDynLib, Debug, TEXT("Unload dynamic library: %ls"), *mName);
#endif
}

void *DynLib::getSymbol(const String &name) const {
    if (!mHandle) {
        return nullptr;
    }
#if PLATFORM != PLATFORM_ANDROID
    return (void *) DYNLIB_GETSYM(mHandle, TCHAR_TO_ANSI(*name));
#else
    return nullptr;
#endif
}

String DynLib::getError() {
#if PLATFORM == PLATFORM_WIN32
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPWSTR) &lpMsgBuf, 0, nullptr);
    String result((LPWSTR) &lpMsgBuf);
    LocalFree(lpMsgBuf);

    return result;
#elif PLATFORM == PLATFORM_LINUX
    return String(lderror());
#else
        return String(_T(""));
#endif
}