#include "BinaryArchive.h"

#include "FileSystem/Stream.h"
#include "Reflection/Class.h"
#include "Reflection/Property.h"
#include "Reflection/Struct.h"

BinaryArchive::BinaryArchive(const TSharedPtr<Stream> &target, EArchiveMode mode) : Archive(target, mode) { }

void BinaryArchive::serialize(void *data, size_t size) {
    if (isSaving()) {
        getTarget()->write(data, size);
    } else {
        getTarget()->read(data, size);
    }
}

Archive &BinaryArchive::operator<<(bool &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

Archive &BinaryArchive::operator<<(int8_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

Archive &BinaryArchive::operator<<(uint8_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

Archive &BinaryArchive::operator<<(int32_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

Archive &BinaryArchive::operator<<(uint32_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

Archive &BinaryArchive::operator<<(int64_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

Archive &BinaryArchive::operator<<(float &value) {
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

Archive &BinaryArchive::operator<<(double &value) {
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

Archive &BinaryArchive::operator<<(uint64_t &value) {
    if (isSaving()) {
        getTarget()->write(&value, sizeof(value));
    } else {
        getTarget()->read(&value, sizeof(value));
    }

    return *this;
}

Archive &BinaryArchive::operator<<(String &value) {
    if (isSaving()) {
        size_t length = value.length();
        getTarget()->write(&length, sizeof(length));
        getTarget()->write(*value, length * sizeof(TCHAR));
    } else {
        size_t length = -1;
        getTarget()->read(&length, sizeof(length));

        if (length > getTarget()->size()) {
            value = String::Empty;
            return *this;
        }

        TCHAR *data = q_alloc<TCHAR>(length);
        getTarget()->read(data, length * sizeof(TCHAR));

        value = String(data, length);
        q_free(data);
    }

    return *this;
}
