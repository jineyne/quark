#include "TextArchive.h"

#include "Reflection/Struct.h"
#include "Reflection/Class.h"
#include "FileSystem/FileStream.h"

TextArchive::TextArchive(const TSharedPtr<Stream> &target, EArchiveMode mode) : Archive(target, mode) { }

static const String Space = String(TEXT(" "));
#define WRITE_TEXT(TEXT) getTarget()->write(TCHAR_TO_ANSI(*(TEXT)), (TEXT).length() * sizeof(ANSICHAR));

Archive &TextArchive::operator<<(bool &value) {
    static const String True = String(TEXT("true"));
    static const String False = String(TEXT("false"));

    if (isSaving()) {
        WRITE_TEXT(value ? True : False);
        WRITE_TEXT(Space);

    } else {
        auto word = getTarget()->readWord();
        if (word == True) {
            value = true;
        } else {
            value = false;
        }

        getTarget()->skip(1); // skip space
    }

    return *this;
}

Archive &TextArchive::operator<<(int8_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%d "), value);
        WRITE_TEXT(converted);
    } else {
        value = CString::Atoi(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

Archive &TextArchive::operator<<(uint8_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%d "), value);
        WRITE_TEXT(converted);
    } else {
        value = CString::Atoi(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

Archive &TextArchive::operator<<(int32_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%ld "), value);
        WRITE_TEXT(converted);
    } else {
        value = CString::Atoi(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

Archive &TextArchive::operator<<(uint32_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%ld "), value);
        WRITE_TEXT(converted);
    } else {
        value = CString::Atoui(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

Archive &TextArchive::operator<<(int64_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%lld "), value);
        WRITE_TEXT(converted);
    } else {
        value = CString::Atoll(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

Archive &TextArchive::operator<<(uint64_t &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%lld "), value);
        WRITE_TEXT(converted);
    } else {
        value = CString::Atoull(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

Archive &TextArchive::operator<<(float &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%f "), value);
        WRITE_TEXT(converted);
    } else {
        value = CString::Atof(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

Archive &TextArchive::operator<<(double &value) {
    if (isSaving()) {
        String converted = String::Printf(TEXT("%f "), value);
        WRITE_TEXT(converted);
    } else {
        value = CString::Atod(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

Archive &TextArchive::operator<<(String &value) {
    WRITE_TEXT(value);

    if (isSaving()) {
        /*String converted = String::Printf(TEXT("%lld "), mValue.length());
        WRITE_TEXT(converted);*/

        /*WRITE_TEXT(Space);*/
    } else {
        /*String word = getTarget()->readWord();
        if (word == TEXT("")) {
            mValue = String::Empty;
            return *this;
        }
        size_t length = CString::Atoll(*word);

        getTarget()->skip(1); // skip space

        auto *ch = new ANSICHAR[length + 1];

        getTarget()->read(ch, length);
        ch[length] = '\0';

        mValue = ANSI_TO_TCHAR(ch);
        delete[] ch;

        getTarget()->skip(1); // skip space*/
    }

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
