#include "BinaryArchive.h"

#include "FileSystem/Stream.h"
#include "Reflection/Class.h"
#include "Reflection/Property.h"
#include "Reflection/Struct.h"

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

FArchive &FBinaryArchive::operator<<(QStruct *value) {
    if (isSaving()) {
        auto fields = value->getClass()->getCppProperties();

        size_t length = fields.length();
        *this << length;

        for (auto field : fields) {
            if (!field->isA<QProperty>()) {
                continue;
            }

            auto property = (QProperty *) field;
            FString &name = const_cast<FString &>(property->getName());

            *this << name;
            property->serializeElement(value, *this);
        }

    } else {
        size_t length = 0;
        *this << length;

        for (int i = 0; i < length; i++) {
            FString name;
            *this << name;
            if (name.empty()) {
                break;
            }

            auto field = value->getClass()->getCppPropertiesByName(name);
            if (field == nullptr) {
                continue;
            }

            if (!field->isA<QProperty>()) {
                continue;
            }

            auto property = (QProperty *) field;
            property->serializeElement(value, *this);
        }
    }

    return *this;
}

FArchive &FBinaryArchive::operator<<(QObject *value) {
    if (isSaving()) {
        auto fields = value->getClass()->getCppProperties();

        size_t length = fields.length();
        *this << length;

        for (auto field : fields) {
            if (!field->isA<QProperty>()) {
                continue;
            }

            auto property = (QProperty *) field;
            FString &name = const_cast<FString &>(property->getName());

            *this << name;
            property->serializeElement(value, *this);
        }
    } else {
        size_t size = value->getClass()->getSize();
        // auto ptr = malloc(size);

        size_t length = 0;
        *this << length;

        // value->getClass()->classConstructor(ptr);

        for (int i = 0; i < length; i++) {
            FString name;
            *this << name;
            if (name.empty()) {
                break;
            }

            auto field = value->getClass()->getCppPropertiesByName(name);
            if (field == nullptr) {
                continue;
            }

            if (!field->isA<QProperty>()) {
                continue;
            }

            auto property = (QProperty *) field;
            property->serializeElement(value, *this);
        }
    }

    return *this;
}

