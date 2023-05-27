#include "String.h"

#include "Misc/CString.h"
#include "Misc/VarArgs.h"
#include "Misc/StringBuilder.h"

template<typename CharType>
static FORCEINLINE void ConstructFromString(TArray<TCHAR> &data, const CharType *src, size_t len) {
    if (src && *src) {
        int srcLen = len > 0 ? len : TCString<CharType>::Strlen(src);
        int dstLen = StringConverter::ConvertedLength<TCHAR>(src, srcLen);

        data.reserve(dstLen + 1);
        data.addUninitialized(dstLen + 1);

        StringConverter::Convert(data.getData(), dstLen, src, srcLen);

        data[dstLen] = '\0';
    }
}

String DLL_EXPORT String::Empty = String(TEXT(""));

String::String(size_t size, TCHAR initializeChar) : mData(size + 1) {
    std::fill(mData.begin(), mData.end(), initializeChar);
    mData[size] = '\0';
}

String::String(const ANSICHAR *src, size_t len) { ConstructFromString(mData, src, len); }
String::String(const WIDECHAR *src, size_t len) { ConstructFromString(mData, src, len); }

// This starting size catches 99.97% of printf calls - there are about 700k printf calls per level
#define STARTING_BUFFER_SIZE		512

String String::Printf(const TCHAR *fmt, ...) {
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

    String resultString(buffer);

    if(bufferSize != STARTING_BUFFER_SIZE) {
        free(buffer);
    }

    return resultString;
}

String String::Vprintf(const TCHAR *fmt, va_list ap) {
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

    String resultString(buffer);

    if(bufferSize != STARTING_BUFFER_SIZE) {
        free(buffer);
    }

    return resultString;
}

TCHAR &String::operator[](size_t index) {
    return mData[index];
}

const TCHAR &String::operator[](size_t index) const {
    return mData[index];
}

bool String::operator==(String &rhs) const {
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

bool String::operator==(const String &rhs) const {
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

bool String::operator!=(String &rhs) const {
    return !(*this == rhs);
}

bool String::operator!=(const String &rhs) const {
    return !(*this == rhs);
}

void String::clear() {
    mData.clear();
}

int32_t String::compare(const String &other) const {
    return CString::Strcmp(getData(), other.getData());
}

bool String::contains(const String &other) const {
    return find(other) != INDEX_NONE;
}

bool String::equals(const String &other) const {
    int32_t len = length();
    int32_t otherLen = other.length();

    if (len != otherLen) {
        return len + otherLen == 1;
    } else  {
        return CString::Strncmp(getData(), other.getData(), len) == 0;
    }
}

bool String::empty() const {
    return length() == 0;
}

int32_t String::find(const String &other, const size_t startPos) const {
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

int32_t String::findChar(TCHAR ch) const {
    for (size_t i = 0; i < mData.length(); i++) {
        if (mData[i] == ch) {
            return i;
        }
    }

    return INDEX_NONE;
}

int32_t String::findLastChar(TCHAR ch) const {
    for (int i = mData.length() - 1; i > 0; i--) {
        if (mData[i] == ch) {
            return i;
        }
    }

    return INDEX_NONE;
}

bool String::isAlnum() const {
    for (auto i = 0; i < length(); i++) {
        auto c = mData[i];
        if (!(CString::IsAlpha(c) && CString::IsNumber(c))) {
            return false;
        }
    }

    return true;
}

String String::lower() const {
    String result(length());

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

void String::reserve(size_t length) {
    check(length > 0);
    mData.reserve(length);
}

void String::replace(String source, String dest) {
    TCHAR *result; // the return string
    TCHAR *ins;    // the next insert point
    int sourceLen;  // length of rep (the string to remove)
    int destLen; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (empty() || source.empty()) {
        return;
    }

    sourceLen = source.length();
    if (sourceLen == 0) {
        return; // empty rep causes infinite loop during count
    }

    destLen = dest.length();

    // count the number of replacements needed
    int at = 0;
    for (count = 0; at = find(source, at), at != INDEX_NONE; ++count) {
        at += sourceLen;
    }

    size_t newLen = (length() + (destLen - sourceLen) * count + 1);
    FStringBuilder sb = FStringBuilder(newLen);

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    int tmp = 0;
    at = 0;
    while (count--) {
        tmp = find(source, at);

        if (tmp != at) {
            sb << mid(at, tmp);
        }
        sb << dest;

        at = tmp + sourceLen;
    }

    sb << right(at);

    *this = sb.toString();
}

TArray<String> String::split(const String &token) const {
    TArray<String> result;
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

String &String::upper() {
    std::transform(mData.begin(), mData.end(), mData.begin(), ::toupper);
    return *this;
}

String &String::lower() {
    std::transform(mData.begin(), mData.end(), mData.begin(), ::tolower);
    return *this;
}

String String::left(int32_t count) const {
    return mid(0, count);
}

String String::mid(int32_t start, int32_t count) const {
    return String((mData.getData() + start), count);
}

String String::right(int32_t start) const {
    return mid(start, length() - start);
}

void String::trim() {
    trimEnd();
    trimStart();
}

void String::trimEnd() {
    size_t index;
    mData.findEndIf([](TCHAR ch) {
        return !CString::IsSpace(ch);
    }, index);

    mData.removeRange(index + 1, length());
}

void String::trimStart() {
    size_t index;
    mData.findIf([](TCHAR ch) {
        return !CString::IsSpace(ch);
    }, index);

    mData.removeRange(0, index);
}

bool String::startWith(const String &token) const {
    if (length() >= token.length()) {
        return (0 == left(token.length()).compare(token));
    } else {
        return false;
    }
}

bool String::endWith(const String &token) const {
    if (length() >= token.length()) {
        return (0 == right(length() - token.length()).compare(token));
    } else {
        return false;
    }
}

std::ostream& operator<< (std::ostream& stream, const String& string) {
    stream << TCHAR_TO_ANSI(*string);
    return stream;
}
