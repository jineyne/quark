#include "Archive.h"

#include "Reflection/Class.h"

FArchive::FArchive(const TSharedPtr<FStream> &target, bool saving) : mTarget(target), bIsSaving(saving) {}

FArchive &FArchive::operator<<(bool &value) {
    if (bIsSaving) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(int8_t &value) {
    if (bIsSaving) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(uint8_t &value) {
    if (bIsSaving) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(int32_t &value) {
    if (bIsSaving) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(uint32_t &value) {
    if (bIsSaving) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(int64_t &value) {
    if (bIsSaving) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(uint64_t &value) {
    if (bIsSaving) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(FString &value) {
    if (bIsSaving) {
        size_t length = value.length();
        mTarget->write(&length, sizeof(length));
        mTarget->write(*value, length * sizeof(TCHAR));
    } else {
        size_t length = -1;
        mTarget->read(&length, sizeof(length));

        TCHAR *data = new TCHAR[length];
        mTarget->read(data, length * sizeof(TCHAR));

        value = data;
        delete[] data;
    }

    return *this;
}

FArchive &FArchive::operator<<(QObject *value) {
    return *this;
}

void FArchive::reset() {
    mTarget->seek(0);
}