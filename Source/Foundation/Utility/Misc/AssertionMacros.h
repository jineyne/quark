#pragma once

#include <cassert>
#include "Prerequisites/PrerequisitesUtil.h"

struct DLL_EXPORT Assertion {
public:
    static void CheckVerifyFailed(const ANSICHAR *file, size_t line, const TCHAR *format, ...);
};

#if !DO_TEST

#define check(EXPR)                                                                         \
{                                                                                           \
    if (!(EXPR)) {                                                                          \
        Assertion::CheckVerifyFailed(__FILE__, __LINE__, TEXT(#EXPR));                     \
        assert(false);                                                                      \
    }                                                                                       \
}

#define checkf(EXPR, FORMAT, ...)                                                           \
{                                                                                           \
    if (!(EXPR)) {                                                                          \
        Assertion::CheckVerifyFailed(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__);           \
        assert(false);                                                                      \
    }                                                                                       \
}

#define verify(EXPR)                                                                        \
{                                                                                           \
    if (!(EXPR)) {                                                                          \
        FAssertion::CheckVerifyFailed(__FILE__, __LINE__, TEXT(#EXPR));                     \
        assert(false);                                                                      \
    }                                                                                       \
}
#define verifyf(EXPR, FORMAT, ...)                                                          \
{                                                                                           \
    if (!(EXPR)) {                                                                          \
        FAssertion::CheckVerifyFailed(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__);           \
        assert(false);                                                                      \
    }                                                                                       \
}

#define unimplemented()             check(!"Unimplemented function called")

#else
#define check(EXPR)                 { assert(EXPR); }
#define checkf(EXPR, FORMAT, ...)   { assert(EXPR); }

#define verify(EXPR)                { if (!(EXPR)) { assert(false); } }
#define verifyf(EXPR, FORMAT, ...)  { if (!(EXPR)) { assert(false && FORMAT); } }

#define unimplemented()				{ assert(false && "unimplemented"); }

#endif

