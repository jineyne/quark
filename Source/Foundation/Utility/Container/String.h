#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Container/Array.h"
#include "Misc/CString.h"
#include "String/StringConverter.h"
#include "ContainerMacros.h"

/**
 * string class
 */
class DLL_EXPORT String {
public:
    using DataType = TArray<TCHAR>;

public:
    static String Empty;

private:
    TArray<TCHAR> mData;

public:
    String() {
        mData.clear();
    }
    String(size_t size, TCHAR initializeChar = TEXT('\0'));

    String(String &&other) : mData() {
        mData = other.mData;
    }
    String(const String &other) : mData() {
        mData = other.mData;
    }

    String(const ANSICHAR *src, size_t len = 0);
    String(const WIDECHAR *src, size_t len = 0);

public:
    static String VARARGS Printf(const TCHAR* fmt, ...);
    static String VARARGS Vprintf(const TCHAR* fmt, va_list ap);

    template <typename T>
    static String ToString(T other);

public:
    FORCEINLINE TCHAR *operator*() {
        return mData.length() ? mData.getData() : Empty.getData();
    }

    FORCEINLINE const TCHAR *operator*() const {
        return mData.length() ? mData.getData() : Empty.getData();
    }

    TCHAR &operator[](size_t index);
    const TCHAR &operator[](size_t index) const;

    String& operator=(String&&) = default;
    String& operator=(const String&) = default;

    bool operator==(String&) const;
    bool operator==(const String&) const;
    bool operator!=(String&) const;
    bool operator!=(const String&) const;

    friend String operator+(const String &str1, const String &str2) {
        String temp = str1;
        return temp.append(str2);
    }

    friend String operator+=(String &str1, const String &str2) {
        return str1.append(str2);
    }

public:
    String &append(const String &string) {
        append(*string, string.length());
        return *this;
    }

    template <typename CharType>
    String &append(CharType *src) {
        append(src, TCString<CharType>::Strlen(src));
        return *this;
    }

    template <typename CharType>
    String &append(const CharType *src, size_t len) {
        if (src && *src) {
            int srcLen = length();
            int dstLen = StringConverter::ConvertedLength<TCHAR>(src, len);

            mData.reserve(srcLen + dstLen + 1);
            if (mData.length() == 0) {
                mData.addUninitialized(dstLen + 1);
            } else {
                // already add null character
                mData.addUninitialized(dstLen);
            }

            StringConverter::Convert(mData.getData() + srcLen, dstLen, src, len);
            mData[length()] = '\0';
        }

        return *this;
    }

    String &appendChar(const TCHAR &c) {
        TCHAR buf[2] = {c, '\0'};
        append(buf, 1);

        return *this;
    }

    void clear();
    int32_t compare(const String &other) const;
    bool contains(const String &other) const;

    bool equals(const String &other) const;

    bool empty() const;
    int32_t find(const String &other, const size_t startPos = 0) const;
    int32_t findChar(TCHAR ch) const;
    int32_t findLastChar(TCHAR ch) const;

    bool isAlnum() const;

    String lower() const;

    /**
     * reserves memory
     */
    void reserve(size_t length);

    // replace source to dest
    void replace(String source, String dest);

    TArray<String> split(const String &token) const;

    const size_t length() const { return mData.length() > 1 ? mData.length() - 1 : 0; }

    String &upper();
    String &lower();

    // from start
    String left(int32_t count) const;
    // middle of string
    String mid(int32_t start, int32_t count) const;
    // to end
    String right(int32_t start) const;

    void trim();
    void trimEnd();
    void trimStart();

    bool startWith(const String &token) const;
    bool endWith(const String &token) const;

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

template<typename T>
String String::ToString(T other) {
    if (std::is_same_v<TCHAR, char>) {
        return String(std::to_string(other).c_str());
    } else if (std::is_same_v<TCHAR, wchar_t>) {
        return String(std::to_wstring(other).c_str());
    }

    return String::Empty;
}

/**	Hash mValue generator for String. */
template<>
struct std::hash<String> {
    size_t operator()(const String &string) const {
        size_t hash_val = 0;
        for (const TCHAR c : string) {
            hash_val = (hash_val << 7) | (hash_val >> (sizeof(size_t) * 8 - 7));
            hash_val ^= c;
        }
        return hash_val;
    }
};