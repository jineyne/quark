#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"
#include "Misc/Platform.h"

#include "CrashHandler.h"

class DLL_EXPORT FException {
private:
    uint32_t mLine = 0;

    FString mType = TEXT("");
    FString mMessage = TEXT("");
    FString mDescription = TEXT("");
    FString mSource = TEXT("");
    FString mFile = TEXT("");

    mutable FString mFullDescription = TEXT("");

public:
    FException(const FString &type, const FString &description, const FString &source);

    FException(const FString &type, const FString &description, const FString &source,
               const FString &file, uint32_t line);

    FException(const FException &rhs);

    ~FException() noexcept = default;

public:
    void operator=(const FException &rhs);

public:
    virtual const FString &getFullDescription() const;

    virtual const FString &getDescription() const { return mDescription; }

    virtual const FString &getSource() const { return mSource; }

    virtual const FString &getFile() const { return mFile; }

    virtual uint32_t getLine() const { return mLine; }
};

class DLL_EXPORT NotImplementedException : public FException {
public:
    NotImplementedException(const FString &description, const FString &source, const FString &file, uint32_t line);
};

class DLL_EXPORT InvalidStateException : public FException {
public:
    InvalidStateException(const FString &description, const FString &source, const FString &file, uint32_t line);
};

class DLL_EXPORT InvalidParametersException : public FException {
public:
    InvalidParametersException(const FString &description, const FString &source, const FString &file, uint32_t line);
};

class DLL_EXPORT InvalidOperationException : public FException {
public:
    InvalidOperationException(const FString &description, const FString &source, const FString &file, uint32_t line);
};

class DLL_EXPORT InternalErrorException : public FException {
public:
    InternalErrorException(const FString &description, const FString &source, const FString &file, uint32_t line);
};

class DLL_EXPORT RenderAPIException : public FException {
public:
    RenderAPIException(const FString &description, const FString &source, const FString &file, uint32_t line);
};

#ifndef EXCEPT
#define PRE_EXCEPT(TAG, TYPE, ...) \
    static_assert(std::is_base_of<FException, TYPE>::value, \
            "Invalid exception type (" #TYPE ") for EXCEPT macro."	   \
			" It needs to derive from FException.");                 \
    gCrashHandler().reportCrash(TEXT(#TYPE), FString::Printf(__VA_ARGS__), ANSI_TO_TCHAR(__PRETTY_FUNCTION__), ANSI_TO_TCHAR(__FILE__), __LINE__); \
    // LOG(TAG, Fatal, *(new TYPE(FString::Printf(__VA_ARGS__), ANSI_TO_TCHAR(__PRETTY_FUNCTION__), ANSI_TO_TCHAR(__FILE__), __LINE__))->getFullDescription());


#if defined (USE_SYSTEM_THROW)
#   define EXCEPT(TAG, TYPE, ...) { PRE_EXCEPT(TAG, TYPE, __VA_ARGS__) throw std::exception("Terminated"); }
#else
#   define EXCEPT(TAG, TYPE, ...) { PRE_EXCEPT(TAG, TYPE, __VA_ARGS__) FPlatform::Terminate(true); }
#endif
#endif
