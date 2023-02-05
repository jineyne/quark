#pragma once

#include "UtilityCore.h"

#if PLATFORM == PLATFORM_WIN32
struct HINSTANCE__;
typedef struct HINSTANCE__ *hInstance;
#endif

#if PLATFORM == PLATFORM_WIN32
#   define DYNLIB_HANDLE hInstance
#   define DYNLIB_LOAD(NAME) LoadLibraryEx(NAME, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH)
#   define DYNLIB_UNLOAD(LIB) !FreeLibrary(LIB)
#   define DYNLIB_GETSYM(LIB, NAME) GetProcAddress(LIB, NAME)
#elif PLATFORM == PLATFORM_LINUX || PLATFORM == PLATFORM_ANDROID
#   define DYNLIB_HANDLE void *
#   define DYNLIB_LOAD(NAME) dlopen(NAME, RTLD_LAZY | RTLD_GLOBAL)
#   define DYNLIB_UNLOAD(LIB) dlclose(LIB)
#   define DYNLIB_GETSYM(LIB, NAME) dlsym(LIB, NAME)
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogDynLib, Info);

class DLL_EXPORT FDynLib final {
public:
#if PLATFORM == PLATFORM_LINUX || PLATFORM == PLATFORM_ANDROID
    static constexpr const TCHAR *Extension = TEXT("so");
#elif PLATFORM == PLATFORM_WIN32
    static constexpr const TCHAR *Extension = TEXT("dll");
#endif

#if PLATFORM == PLATFORM_LINUX || PLATFORM == PLATFORM_ANDROID
    static constexpr const TCHAR *Prefix = TEXT("lib");
#elif PLATFORM == PLATFORM_WIN32
    static constexpr const TCHAR *Prefix = nullptr;
#endif
private:
    const FString mName;

    DYNLIB_HANDLE mHandle = nullptr;

public:
    FDynLib(const FString &name);
    ~FDynLib();

public:
    void load();
    void unload();

    const FString &getName() const { return mName; }
    void *getSymbol(const FString &name) const;

private:
    FString getError();
};
