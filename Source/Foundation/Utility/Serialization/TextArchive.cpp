#include "TextArchive.h"

#include "FileSystem/FileStream.h"

FTextArchive::FTextArchive(const TSharedPtr<FStream> &target, EArchiveMode mode) : FArchive(target, mode) { }

static const FString Space = FString(TEXT(" "));
#define WRITE_TEXT(TEXT) getTarget()->write(TCHAR_TO_ANSI(*(TEXT)), (TEXT).length() * sizeof(ANSICHAR));

FArchive &FTextArchive::operator<<(bool &value) {
    static const FString True = FString(TEXT("true"));
    static const FString False = FString(TEXT("false"));

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

FArchive &FTextArchive::operator<<(int8_t &value) {
    if (isSaving()) {
        FString converted = FString::Printf(TEXT("%d "), value);
        WRITE_TEXT(converted);
    } else {
        value = FCString::Atoi(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

FArchive &FTextArchive::operator<<(uint8_t &value) {
    if (isSaving()) {
        FString converted = FString::Printf(TEXT("%d "), value);
        WRITE_TEXT(converted);
    } else {
        value = FCString::Atoi(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

FArchive &FTextArchive::operator<<(int32_t &value) {
    if (isSaving()) {
        FString converted = FString::Printf(TEXT("%ld "), value);
        WRITE_TEXT(converted);
    } else {
        value = FCString::Atoi(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

FArchive &FTextArchive::operator<<(uint32_t &value) {
    if (isSaving()) {
        FString converted = FString::Printf(TEXT("%ld "), value);
        WRITE_TEXT(converted);
    } else {
        value = FCString::Atoui(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

FArchive &FTextArchive::operator<<(int64_t &value) {
    if (isSaving()) {
        FString converted = FString::Printf(TEXT("%lld "), value);
        WRITE_TEXT(converted);
    } else {
        value = FCString::Atoll(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

FArchive &FTextArchive::operator<<(uint64_t &value) {
    if (isSaving()) {
        FString converted = FString::Printf(TEXT("%lld "), value);
        WRITE_TEXT(converted);
    } else {
        value = FCString::Atoull(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

FArchive &FTextArchive::operator<<(float &value) {
    if (isSaving()) {
        FString converted = FString::Printf(TEXT("%f "), value);
        WRITE_TEXT(converted);
    } else {
        value = FCString::Atof(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

FArchive &FTextArchive::operator<<(double &value) {
    if (isSaving()) {
        FString converted = FString::Printf(TEXT("%f "), value);
        WRITE_TEXT(converted);
    } else {
        value = FCString::Atod(*(getTarget()->readWord()));
        getTarget()->skip(1); // skip space
    }

    return *this;
}

FArchive &FTextArchive::operator<<(FString &value) {
    WRITE_TEXT(value);

    if (isSaving()) {
        /*FString converted = FString::Printf(TEXT("%lld "), value.length());
        WRITE_TEXT(converted);*/

        /*WRITE_TEXT(Space);*/
    } else {
        /*FString word = getTarget()->readWord();
        if (word == TEXT("")) {
            value = FString::Empty;
            return *this;
        }
        size_t length = FCString::Atoll(*word);

        getTarget()->skip(1); // skip space

        auto *ch = new ANSICHAR[length + 1];

        getTarget()->read(ch, length);
        ch[length] = '\0';

        value = ANSI_TO_TCHAR(ch);
        delete[] ch;

        getTarget()->skip(1); // skip space*/
    }

    return *this;
}