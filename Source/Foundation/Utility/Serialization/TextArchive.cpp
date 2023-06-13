#include "TextArchive.h"

#include "Reflection/Struct.h"
#include "Reflection/Class.h"
#include "FileSystem/FileStream.h"

TextArchive::TextArchive(const TSharedPtr<Stream> &target, EArchiveMode mode) : Archive(target, mode) {
    checkf(mode != EArchiveMode::Load, TEXT("TextArchive only support save!"));
}

static const String Space = String(TEXT(" "));
#define WRITE_TEXT(TEXT) getTarget()->write(TCHAR_TO_ANSI(*(TEXT)), (TEXT).length() * sizeof(ANSICHAR));

Archive &TextArchive::operator<<(bool &value) {
    static const String True = String(TEXT("true"));
    static const String False = String(TEXT("false"));

    WRITE_TEXT(value ? True : False);
    WRITE_TEXT(Space);

    return *this;
}

Archive &TextArchive::operator<<(int8_t &value) {

    String converted = String::Printf(TEXT("%d "), value);
    WRITE_TEXT(converted);

    return *this;
}

Archive &TextArchive::operator<<(uint8_t &value) {
    String converted = String::Printf(TEXT("%d "), value);
    WRITE_TEXT(converted);

    return *this;
}

Archive &TextArchive::operator<<(int32_t &value) {
    String converted = String::Printf(TEXT("%ld "), value);
    WRITE_TEXT(converted);

    return *this;
}

Archive &TextArchive::operator<<(uint32_t &value) {
    String converted = String::Printf(TEXT("%ld "), value);
    WRITE_TEXT(converted);

    return *this;
}

Archive &TextArchive::operator<<(int64_t &value) {
    String converted = String::Printf(TEXT("%lld "), value);
    WRITE_TEXT(converted);

    return *this;
}

Archive &TextArchive::operator<<(uint64_t &value) {
    String converted = String::Printf(TEXT("%lld "), value);
    WRITE_TEXT(converted);

    return *this;
}

Archive &TextArchive::operator<<(float &value) {
    String converted = String::Printf(TEXT("%f "), value);
    WRITE_TEXT(converted);

    return *this;
}

Archive &TextArchive::operator<<(double &value) {
    String converted = String::Printf(TEXT("%f "), value);
    WRITE_TEXT(converted);

    return *this;
}

Archive &TextArchive::operator<<(String &value) {
    WRITE_TEXT(value);

    return *this;
}

Archive &TextArchive::operator<<(Struct *value) {
    value->serialize(*this);
    return *this;
}

Archive &TextArchive::operator<<(Object *value) {
    value->serialize(*this);
    return *this;
}
