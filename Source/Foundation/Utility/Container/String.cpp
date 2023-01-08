#include "String.h"

#include "Misc/CString.h"
#include "Misc/VarArgs.h"

template<typename CharType>
static FORCEINLINE void ConstructFromString(TArray<TCHAR> &data, const CharType *src, size_t len) {
    if (src && *src) {
        int srcLen = len > 0 ? len : TCString<CharType>::Strlen(src);
        int dstLen = FStringConverter::ConvertedLength<TCHAR>(src, srcLen);

        data.reserve(dstLen + 1);
        data.addUninitialized(dstLen + 1);

        FStringConverter::Convert(data.getData(), dstLen, src, srcLen);

        data[dstLen] = '\0';
    }
}

FString FString::Empty = FString(TEXT(""));

FString::FString(size_t size, TCHAR initializeChar) : mData(size + 1) {
    std::fill(mData.begin(), mData.end(), initializeChar);
    mData[size] = '\0';
}

FString::FString(const ANSICHAR *src, size_t len) { ConstructFromString(mData, src, len); }
FString::FString(const WIDECHAR *src, size_t len) { ConstructFromString(mData, src, len); }

// This starting size catches 99.97% of printf calls - there are about 700k printf calls per level
#define STARTING_BUFFER_SIZE		512

FString FString::Printf(const TCHAR *fmt, ...) {
    int32_t bufferSize = STARTING_BUFFER_SIZE;
    TCHAR startingBuffer[STARTING_BUFFER_SIZE];
    TCHAR *buffer = startingBuffer;
    int32_t result;

    GET_VARARGS_RESULT(buffer, bufferSize, bufferSize - 1, fmt, fmt, result);

    if (result == -1) {
        buffer = nullptr;
        while (result == -1) {
            bufferSize *= 2;
            buffer = (TCHAR *) realloc(buffer, bufferSize * sizeof(TCHAR));

            GET_VARARGS_RESULT(buffer, bufferSize, bufferSize - 1, fmt, fmt, result);
        }
    }

    buffer[result] = TEXT('\0');

    FString resultString(buffer);

    if(bufferSize != STARTING_BUFFER_SIZE) {
        free(buffer);
    }

    return resultString;
}

FString FString::Vprintf(const TCHAR *fmt, va_list ap) {
    int32_t bufferSize = STARTING_BUFFER_SIZE;
    TCHAR startingBuffer[STARTING_BUFFER_SIZE];
    TCHAR *buffer = startingBuffer;
    int32_t result;

    GET_VARARGS_RESULT_VALIST(buffer, bufferSize, bufferSize - 1, fmt, fmt, result, ap);

    if (result == -1) {
        buffer = nullptr;
        while (result == -1) {
            bufferSize *= 2;
            buffer = (TCHAR *) realloc(buffer, bufferSize * sizeof(TCHAR));

            GET_VARARGS_RESULT_VALIST(buffer, bufferSize, bufferSize - 1, fmt, fmt, result, ap);
        }
    }

    buffer[result] = TEXT('\0');

    FString resultString(buffer);

    if(bufferSize != STARTING_BUFFER_SIZE) {
        free(buffer);
    }

    return resultString;
}

TCHAR &FString::operator[](size_t index) {
    return mData[index];
}

const TCHAR &FString::operator[](size_t index) const {
    return mData[index];
}

bool FString::operator==(FString &rhs) const {
    if (length() != rhs.length()) {
        return false;
    }

    for (size_t i = 0; i < length(); i++) {
        if ((*this)[i] != rhs[i]) {
            return false;
        }
    }

    return true;
}

bool FString::operator==(const FString &rhs) const {
    if (length() != rhs.length()) {
        return false;
    }

    for (size_t i = 0; i < length(); i++) {
        if ((*this)[i] != rhs[i]) {
            return false;
        }
    }

    return true;
}

bool FString::operator!=(FString &rhs) const {
    return !(*this == rhs);
}

bool FString::operator!=(const FString &rhs) const {
    return !(*this == rhs);
}

void FString::clear() {
    mData.clear();
}

int32_t FString::compare(const FString &other) const {
    return FCString::Strcmp(getData(), other.getData());
}

bool FString::contains(const FString &other) const {
    return find(other) == INDEX_NONE;
}

bool FString::equals(const FString &other) const {
    int32_t len = length();
    int32_t otherLen = other.length();

    if (len != otherLen) {
        return len + otherLen == 1;
    } else  {
        return FCString::Strncmp(getData(), other.getData(), len) == 0;
    }
}

bool FString::empty() const {
    return length() == 0;
}

int32_t FString::find(const FString &other, const size_t startPos) const {
    for (size_t i = startPos; i < mData.length(); i++) {
        for (size_t j = 0; j < other.length(); j++) {
            if (mData[i + j] != other[j]) {
                goto not_same;
            }
        }

        return static_cast<int>(i);

        not_same:
        continue;
    }

    return INDEX_NONE;
}

int32_t FString::findChar(TCHAR ch) const {
    for (size_t i = 0; i < mData.length(); i++) {
        if (mData[i] == ch) {
            return i;
        }
    }

    return INDEX_NONE;
}

int32_t FString::findLastChar(TCHAR ch) const {
    for (int i = mData.length() - 1; i > 0; i--) {
        if (mData[i] == ch) {
            return i;
        }
    }

    return INDEX_NONE;
}

bool FString::isAlnum() const {
    for (auto i = 0; i < length(); i++) {
        auto c = mData[i];
        if (!(FCString::IsAlpha(c) && FCString::IsNumber(c))) {
            return false;
        }
    }

    return true;
}

FString FString::lower() const {
    FString result(length());

    for (auto i = 0; i < length(); i++) {
        TCHAR c = mData[i];

        if (std::isalpha(c)) {
            result[i] = std::tolower(c);
        } else {
            result[i] = c;
        }
    }

    return result;
}

void FString::reserve(size_t length) {
    check(length > 0);
    mData.reserve(length);
}

TArray<FString> FString::split(const FString &token) const {
    TArray<FString> result;
    size_t startPos = 0;
    size_t findPos = 0;
    while ((findPos = find(token, startPos)) != INDEX_NONE) {
        result.push(mid(startPos, findPos - startPos));
        startPos = findPos + token.length();
    }

    if (startPos < length()) {
        result.push(mid(startPos, length() - startPos));
    }

    return result;
}

FString FString::mid(int32_t start, int32_t count) const {
    return FString((mData.getData() + start), count);
}

std::ostream& operator<< (std::ostream& stream, const FString& string) {
    stream << TCHAR_TO_ANSI(*string);
    return stream;
}
