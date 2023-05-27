#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"
#include "Misc/Platform.h"

#include "CrashHandler.h"

class DLL_EXPORT FException {
private:
    uint32_t mLine = 0;

    String mType = TEXT("");
    String mMessage = TEXT("");
    String mDescription = TEXT("");
    String mSource = TEXT("");
    String mFile = TEXT("");

    mutable String mFullDescription = TEXT("");

public:
    FException(const String &type, const String &description, const String &source);

    FException(const String &type, const String &description, const String &source,
               const String &file, uint32_t line);

    FException(const FException &rhs);

    ~FException() noexcept = default;

public:
    void operator=(const FException &rhs);

public:
    virtual const String &getFullDescription() const;

    virtual const String &getDescription() const { return mDescription; }

    virtual const String &getSource() const { return mSource; }

    virtual const String &getFile() const { return mFile; }

    virtual uint32_t getLine() const { return mLine; }
};

class DLL_EXPORT NotImplementedException : public FException {
public:
    NotImplementedException(const String &description, const String &source, const String &file, uint32_t line);
};

class DLL_EXPORT InvalidStateException : public FException {
public:
    InvalidStateException(const String &description, const String &source, const String &file, uint32_t line);
};

class DLL_EXPORT InvalidParametersException : public FException {
public:
    InvalidParametersException(const String &description, const String &source, const String &file, uint32_t line);
};

class DLL_EXPORT InvalidOperationException : public FException {
public:
    InvalidOperationException(const String &description, const String &source, const String &file, uint32_t line);
};

class DLL_EXPORT InternalErrorException : public FException {
public:
    InternalErrorException(const String &description, const String &source, const String &file, uint32_t line);
};

class DLL_EXPORT RenderAPIException : public FException {
public:
    RenderAPIException(const String &description, const String &source, const String &file, uint32_t line);
};

#ifndef EXCEPT
#define PRE_EXCEPT(TAG, TYPE, ...) \
    static_assert(std::is_base_of<FException, TYPE>::value, \
            "Invalid exception type (" #TYPE ") for EXCEPT macro."	   \
			" It needs to derive from FException.");                 \
    gCrashHandler().reportCrash(TEXT(#TYPE), String::Printf(__VA_ARGS__), ANSI_TO_TCHAR(__PRETTY_FUNCTION__), ANSI_TO_TCHAR(__FILE__), __LINE__); \
    // LOG(TAG, Fatal, *(new TYPE(String::Printf(__VA_ARGS__), ANSI_TO_TCHAR(__PRETTY_FUNCTION__), ANSI_TO_TCHAR(__FILE__), __LINE__))->getFullDescription());


#if defined (USE_SYSTEM_THROW)
#   define EXCEPT(TAG, TYPE, ...) { PRE_EXCEPT(TAG, TYPE, __VA_ARGS__) throw std::exception("Terminated"); }
#else
#   define EXCEPT(TAG, TYPE, ...) { PRE_EXCEPT(TAG, TYPE, __VA_ARGS__) Platform::Terminate(true); }
#endif
#endif
