#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Container/String.h"
#include "Memory/MemoryUtil.h"
#include "Traits/CharTraits.h"

template <typename CharType>
class TStringBuilder {
protected:
    static inline CharType EmptyBuffer[1]{};
    THeapAllocator<CharType> mAllocation;

    CharType *mData = EmptyBuffer;
    CharType *mCursorPos = mData;
    CharType *mEnd = mData + 1;
    bool bIsDynamic = false;

public:
    inline TStringBuilder(uint32_t capacity) {
        mAllocation.resize(0, capacity, sizeof(CharType));
        mData = mAllocation.getAllocation();
        mCursorPos = mData;
        mEnd = mData + capacity;
    }

    inline TStringBuilder(CharType* ptr, uint32_t capacity) {
        mAllocation.resize(0, capacity, sizeof(CharType));

        mData = mAllocation.getAllocation();
        mCursorPos = mData;
        mEnd = mData + capacity;

        append(ptr, capacity);
    }

    ~TStringBuilder() {
    }

public:
    template <typename U, typename = typename std::enable_if_t<TIsCharType<U>::Value>>
    inline TStringBuilder &append(const U * const string, size_t length) {
        checkSizeAndResize(length);
        memcpy(mCursorPos, string, sizeof(U) * length);
        mCursorPos += length;

        *mCursorPos = '\0';

        return *this;
    }

    inline TStringBuilder &appendChar(CharType Char) {
        checkSizeAndResize(1);
        *(mCursorPos++) = Char;
        *mCursorPos = '\0';
        return *this;
    }

    template <typename U>
    inline TStringBuilder & VARARGS appendF(const U *fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        auto string = FString::Vprintf(fmt, ap);
        va_end(ap);

        append(*string, string.length());

        return* this;
    }

    inline void reset() {
        mCursorPos = mData;
    }

    FString toString() const {
        return FString(mData, length());
    }

    void setDynamic() { bIsDynamic = true; }

    CharType *getData() { return mData; }
    const CharType *getData() const { return mData; }
    inline uint32_t length() const { return uint32_t(mCursorPos - mData); }
    inline uint32_t capacity() const { return uint32_t(mEnd - mData); }

protected:
    inline void checkSizeAndResize(size_t size) {
        if ((mCursorPos + size) < mEnd) {
            return;
        }

        extend(size);
    }

    void extend(size_t size) {
        checkf(bIsDynamic, TEXT("This StringBuffer is static!"));

        size_t len = length();
        size_t required = len;

        while (required <= size + len) {
            required *= 2;
        }

        mAllocation.resize(0, required, sizeof(CharType));

        mData = mAllocation.getAllocation();
        mCursorPos = mData + len;
        mEnd = mData + size;
    }
};

using FStringBuilder = TStringBuilder<TCHAR>;

inline FStringBuilder &operator<<(FStringBuilder &builder, TCHAR ch) { return builder.appendChar(ch); }
inline FStringBuilder &operator<<(FStringBuilder &builder, const FString &str) { return builder.append(*str, str.length()); }
inline FStringBuilder &operator<<(FStringBuilder &builder, const TCHAR *str) { return builder.append(str, FString(str).length()); }

inline FStringBuilder &operator<<(FStringBuilder &builder, int32_t value) { return builder.appendF(TEXT("%d"), value); }
inline FStringBuilder &operator<<(FStringBuilder &builder, uint32_t value) { return builder.appendF(TEXT("%u"), value); }
inline FStringBuilder &operator<<(FStringBuilder &builder, int64_t value) { return builder.appendF(TEXT("%ld"), value); }
inline FStringBuilder &operator<<(FStringBuilder &builder, uint64_t value) { return builder.appendF(TEXT("%lu"), value); }

inline FStringBuilder &operator<<(FStringBuilder &builder, int8_t value) { return builder << int32_t(value); }
inline FStringBuilder &operator<<(FStringBuilder &builder, uint8_t value) { return builder << uint32_t(value); }
inline FStringBuilder &operator<<(FStringBuilder &builder, int16_t value) { return builder << int32_t(value); }
inline FStringBuilder &operator<<(FStringBuilder &builder, uint16_t value) { return builder << uint32_t(value); }