#include "TextArchive.h"

#include "FileSystem/FileStream.h"

FTextArchive::FTextArchive(const TSharedPtr<FStream> &target, EArchiveMode mode) : FArchive(target, mode) { }

FArchive &FTextArchive::operator<<(bool &value) {
    return *this;
}

FArchive &FTextArchive::operator<<(int8_t &value) {
    return *this;
}

FArchive &FTextArchive::operator<<(uint8_t &value) {
    return *this;
}

FArchive &FTextArchive::operator<<(int32_t &value) {
    return *this;
}

FArchive &FTextArchive::operator<<(uint32_t &value) {
    return *this;
}

FArchive &FTextArchive::operator<<(int64_t &value) {
    return *this;
}

FArchive &FTextArchive::operator<<(uint64_t &value) {
    return *this;
}

FArchive &FTextArchive::operator<<(float &value) {
    return *this;
}

FArchive &FTextArchive::operator<<(double &value) {
    return *this;
}

FArchive &FTextArchive::operator<<(FString &value) {
    if (isSaving()) {
        getTarget()->write(TCHAR_TO_ANSI(*value), value.length() * sizeof(ANSICHAR));
    } else {
        size_t length = getTarget()->size();
        auto *ch = new ANSICHAR[length + 1];

        getTarget()->read(ch, length);
        ch[length] = '\0';

        value = ANSI_TO_TCHAR(ch);
        delete[] ch;
    }

    return *this;
}

FArchive &FTextArchive::operator<<(struct QObject *value) {
    return *this;
}
