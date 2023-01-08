#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Misc/Char.h"
#include "Misc/VarArgs.h"

#define MAX_SPRINTF 1024

template <typename T>
struct TCString {
    using CharType = T;

    /**
	 * strlen wrapper
	 */
    static FORCEINLINE int32_t Strlen(const CharType *string);

    /**
     * strcpy wrapper
     */
    static FORCEINLINE CharType *Strcpy(CharType *dest, const CharType *source, size_t length);

    /**
     * strcmp wrapper
     */
    static FORCEINLINE int32_t Strcmp(const CharType *s1, const CharType *s2);

    /**
     * strncmp wrapper
     */
    static FORCEINLINE int32_t Strncmp(const CharType *s1, const CharType *s2, size_t count);

    static FORCEINLINE bool IsAlpha(CharType c);
    static FORCEINLINE bool IsNumber(CharType c);

    static FORCEINLINE int32_t VARARGS Sprintf(CharType* dest, size_t maxSize, const CharType* fmt, ...);
    static FORCEINLINE int32_t VARARGS Vsprintf(CharType* dest, size_t maxSize, const CharType* fmt, va_list vp);
};

template <typename T> FORCEINLINE
int32_t TCString<T>::Strlen(const CharType *string) {
    if (!string) { return 0; }

    int Len = 0;
    while (string[Len]) {
        ++Len;
    }

    return Len;
}

template <typename T> FORCEINLINE
typename TCString<T>::CharType *TCString<T>::Strcpy(CharType *dest, const CharType *source, size_t length) {
    check(dest);
    check(source);

    size_t i = 0;
    while (i < length) {
        dest[i] = source[i];
        i++;
    }

    return dest;
}

template <typename T> FORCEINLINE
int32_t TCString<T>::Strcmp(const CharType *s1, const CharType *s2) {
    CharType *t1 = const_cast<CharType *>(s1);
    CharType *t2 = const_cast<CharType *>(s2);

    while (*t1 != '\0' || *t2 != '\0') {
        if (*t1 > *t2) {
            return 1;
        } else if (*t1 < *t2) {
            return -1;
        } else {
            t1++;
            t2++;
        }
    }

    return 0;
}

template <typename T> FORCEINLINE
int32_t TCString<T>::Strncmp(const CharType *s1, const CharType *s2, size_t count) {
    CharType *t1 = const_cast<CharType *>(s1);
    CharType *t2 = const_cast<CharType *>(s2);

    while ((*t1 != '\0' || *t2 != '\0') && count > 0) {
        if (*t1 > *t2) {
            return 1;
        } else if (*t1 < *t2) {
            return -1;
        } else {
            t1++;
            t2++;
            count--;
        }
    }

    return 0;
}

template<typename T>
bool TCString<T>::IsAlpha(CharType c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

template<typename T>
bool TCString<T>::IsNumber(CharType c) {
    return ('0' <= c && c <= '9');
}

template<> FORCEINLINE
int32_t TCString<ANSICHAR>::Sprintf(CharType *dest, size_t maxSize, const CharType *fmt, ...) {
    va_list ap;
            va_start(ap, fmt);
    int result = vsprintf(dest, fmt, ap);
    if (result >= maxSize) {
        return -1;
    }

    return result;
}

template<> FORCEINLINE
int32_t TCString<WIDECHAR>::Sprintf(CharType *dest, size_t maxSize, const CharType *fmt, ...) {
    va_list ap;
            va_start(ap, fmt);
    int result = vswprintf(dest, maxSize, fmt, ap);
    if (result >= maxSize) {
        return -1;
    }

    return result;
}

template<> FORCEINLINE
int32_t TCString<ANSICHAR>::Vsprintf(CharType *dest, size_t maxSize, const CharType *fmt, va_list ap) {
    int result = vsprintf(dest, fmt, ap);
    if (result >= maxSize) {
        return -1;
    }

    return result;
}

template<> FORCEINLINE
int32_t TCString<WIDECHAR>::Vsprintf(CharType *dest, size_t maxSize, const CharType *fmt, va_list ap) {
    int result = vswprintf(dest, maxSize, fmt, ap);
    if (result >= maxSize) {
        return -1;
    }

    return result;
}

using FCString = TCString<TCHAR>;
using FCStringAnsi = TCString<ANSICHAR>;
using FCStringWide = TCString<WIDECHAR>;
