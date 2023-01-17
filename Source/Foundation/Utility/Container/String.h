#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Container/Array.h"
#include "Misc/CString.h"
#include "String/StringConverter.h"
#include "ContainerMacros.h"

/**
 * string class
 */
class DLL_EXPORT FString {
public:
    using DataType = TArray<TCHAR>;

public:
    static FString Empty;

private:
    TArray<TCHAR> mData;

public:
    FString() {
        mData.clear();
    }
    FString(size_t size, TCHAR initializeChar = TEXT('\0'));

    FString(FString &&other) : mData() {
        mData = other.mData;
    }
    FString(const FString &other) : mData() {
        mData = other.mData;
    }

    FString(const ANSICHAR *src, size_t len = 0);
    FString(const WIDECHAR *src, size_t len = 0);

public:
    static FString VARARGS Printf(const TCHAR* fmt, ...);
    static FString VARARGS Vprintf(const TCHAR* fmt, va_list ap);

public:
    FORCEINLINE TCHAR *operator*() {
        return mData.length() ? mData.getData() : Empty.getData();
    }

    FORCEINLINE const TCHAR *operator*() const {
        return mData.length() ? mData.getData() : Empty.getData();
    }

    TCHAR &operator[](size_t index);
    const TCHAR &operator[](size_t index) const;

    FString& operator=(FString&&) = default;
    FString& operator=(const FString&) = default;

    bool operator==(FString&) const;
    bool operator==(const FString&) const;
    bool operator!=(FString&) const;
    bool operator!=(const FString&) const;

public:
    FString &append(const FString &string) {
        append(*string, string.length());
        return *this;
    }

    template <typename CharType>
    FString &append(CharType *src) {
        append(src, TCString<CharType>::Strlen(src));
        return *this;
    }

    template <typename CharType>
    FString &append(const CharType *src, size_t len) {
        if (src && *src) {
            int srcLen = length();
            int dstLen = FStringConverter::ConvertedLength<TCHAR>(src, len);

            mData.reserve(mData.length() + dstLen);
            mData.addUninitialized(dstLen);

            FStringConverter::Convert(mData.getData() + srcLen, dstLen, src, len);
        }

        return *this;
    }

    void clear();
    int32_t compare(const FString &other) const;
    bool contains(const FString &other) const;

    bool equals(const FString &other) const;

    bool empty() const;
    int32_t find(const FString &other, const size_t startPos = 0) const;
    int32_t findChar(TCHAR ch) const;
    int32_t findLastChar(TCHAR ch) const;

    bool isAlnum() const;

    FString lower() const;

    /**
     * reserves memory
     */
    void reserve(size_t length);

    TArray<FString> split(const FString &token) const;

    const size_t length() const { return mData.length() ? mData.length() - 1 : 0; }

    FString mid(int32_t start,int32_t count) const;

    TCHAR *getData() { return mData.getData(); }
    const TCHAR *getData() const { return mData.getData(); }

    typename DataType::Internal::iterator begin() { return mData.begin(); }
    typename DataType::Internal::const_iterator begin() const { return mData.begin(); }

    typename DataType::Internal::iterator end() { return mData.end(); }
    typename DataType::Internal::const_iterator end() const { return mData.end(); }

    typename DataType::Internal::reverse_iterator rbegin() { return mData.rbegin(); }
    typename DataType::Internal::const_reverse_iterator rbegin() const { return mData.rbegin(); }

    typename DataType::Internal::reverse_iterator rend() { return mData.rend(); }
    typename DataType::Internal::const_reverse_iterator rend() const { return mData.rend(); }
};

/**	Hash value generator for Uuid. */
template<>
struct std::hash<FString> {
    size_t operator()(const FString &string) const {
        return std::hash<const TCHAR *>()(string.getData());
    }
};