#include "TextArchive.h"

#include "Reflection/Struct.h"
#include "Reflection/Class.h"
#include "FileSystem/FileStream.h"

TextArchive::TextArchive(const TSharedPtr<Stream> &target, EArchiveMode mode) : Archive(target, mode) {
    // checkf(mode != EArchiveMode::Load, TEXT("TextArchive only support save!"));
}

static const String Space = String(TEXT(" "));
#define WRITE_TEXT(TEXT) getTarget()->write(TCHAR_TO_ANSI(*(TEXT)), (TEXT).length() * sizeof(ANSICHAR));

void TextArchive::serialize(void *data, size_t size) {
    assert(false);

    /*TCHAR *encoded;
    Base64::Encode((TCHAR *) data, size, &encoded);

    WRITE_TEXT(String(encoded, size));*/
}

Archive &TextArchive::operator<<(bool &value) {
    static const String True = String(TEXT("true"));
    static const String False = String(TEXT("false"));

    if (isSaving()) {
        WRITE_TEXT(value ? True : False);
        WRITE_TEXT(Space);
    }

    return *this;
}

Archive &TextArchive::operator<<(int8_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%d "), value);
        WRITE_TEXT(converted);
    }

    return *this;
}

Archive &TextArchive::operator<<(uint8_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%d "), value);
        WRITE_TEXT(converted);
    }

    return *this;
}

Archive &TextArchive::operator<<(int32_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%ld "), value);
        WRITE_TEXT(converted);
    }

    return *this;
}

Archive &TextArchive::operator<<(uint32_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%ld "), value);
        WRITE_TEXT(converted);
    }

    return *this;
}

Archive &TextArchive::operator<<(int64_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%lld "), value);
        WRITE_TEXT(converted);
    }

    return *this;
}

Archive &TextArchive::operator<<(uint64_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%lld "), value);
        WRITE_TEXT(converted);
    }

    return *this;
}

Archive &TextArchive::operator<<(float &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%f "), value);
        WRITE_TEXT(converted);
    }

    return *this;
}

Archive &TextArchive::operator<<(double &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%f "), value);
        WRITE_TEXT(converted);
    }

    return *this;
}

Archive &TextArchive::operator<<(String &value) {
    if (isSaving()) {
        WRITE_TEXT(value);
    } else {
        auto size = getTarget()->size();
        char *data = q_alloc<char>(size + 1);
        getTarget()->read(data, size);
        data[size] = '\0';
        value = ANSI_TO_TCHAR(data);
    }

    return *this;
}
