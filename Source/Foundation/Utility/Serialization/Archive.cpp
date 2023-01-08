#include "Archive.h"

#include "Reflection/Class.h"
#include "Reflection/Property.h"

FArchive::FArchive(const TSharedPtr<FStream> &target, EArchiveMode mode) : mTarget(target), mMode(mode) {}

FArchive::operator bool() {
    return !mTarget->eof();
}

FArchive &FArchive::operator<<(bool &value) {
    if (isSaving()) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(int8_t &value) {
    if (isSaving()) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(uint8_t &value) {
    if (isSaving()) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(int32_t &value) {
    if (isSaving()) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(uint32_t &value) {
    if (isSaving()) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(int64_t &value) {
    if (isSaving()) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(float &value) {
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

FArchive &FArchive::operator<<(double &value) {
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

FArchive &FArchive::operator<<(uint64_t &value) {
    if (isSaving()) {
        mTarget->write(&value, sizeof(value));
    } else {
        mTarget->read(&value, sizeof(value));
    }

    return *this;
}

FArchive &FArchive::operator<<(FString &value) {
    if (isSaving()) {
        size_t length = value.length();
        mTarget->write(&length, sizeof(length));
        mTarget->write(*value, length * sizeof(TCHAR));
    } else {
        size_t length = -1;
        mTarget->read(&length, sizeof(length));

        if (length > mTarget->size()) {
            value = FString::Empty;
            return *this;
        }

        TCHAR *data = new TCHAR[length];
        mTarget->read(data, length * sizeof(TCHAR));

        value = FString(data, length);
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