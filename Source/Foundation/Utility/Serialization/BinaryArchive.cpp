#include "BinaryArchive.h"

#include "FileSystem/Stream.h"

FBinaryArchive::FBinaryArchive(const TSharedPtr<FStream> &target, EArchiveMode mode) : FArchive(target, mode) { }

FArchive &FBinaryArchive::operator<<(bool &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FBinaryArchive::operator<<(int8_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FBinaryArchive::operator<<(uint8_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FBinaryArchive::operator<<(int32_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FBinaryArchive::operator<<(uint32_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FBinaryArchive::operator<<(int64_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FBinaryArchive::operator<<(float &value) {
    union
    {
        float f;
        int   i;
    } data;

    if (isSaving()) {
        data.f = value;
        *this << data.i;
    } else {
        *this << data.i;
        value = data.f;
    }

    return *this;
}

FArchive &FBinaryArchive::operator<<(double &value) {
    union
    {
        double  d;
        int64_t i;
    } data;

    if (isSaving()) {
        data.d = value;
        *this << data.i;
    } else {
        *this << data.i;
        value = data.d;
    }

    return *this;
}

FArchive &FBinaryArchive::operator<<(uint64_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FBinaryArchive::operator<<(FString &value) {
    if (isSaving()) {
        size_t length = value.length();
        getTarget()->write(&length, sizeof(length));
        getTarget()->write(*value, length * sizeof(TCHAR));
    } else {
        size_t length = -1;
        getTarget()->read(&length, sizeof(length));

        if (length > getTarget()->size()) {
            value = FString::Empty;
            return *this;
        }

        TCHAR *data = new TCHAR[length];
        getTarget()->read(data, length * sizeof(TCHAR));

        value = FString(data, length);
        delete[] data;
    }

    return *this;
}
