#include "Enum.h"

#include "Reflection.h"

TMap<String, Enum *> Enum::AllEnumByNameMap;

Enum::Enum(Class *myClass, const String &name)
        : Field(myClass, name) {}

bool Enum::LookupEnum(const String &name, Enum **found) {
    for (auto pair : AllEnumByNameMap) {
        if (pair.key.contains(name)) {
            *found = pair.value;
            return true;
        }
    }

    return false;
}

int64_t Enum::LookupEnumName(const String &name, Enum **found) {
    auto it = AllEnumByNameMap.find(name);
    if (it == nullptr) {
        return std::numeric_limits<int64_t>::max();
    }

    *found = *it;
    return (*found != nullptr) ? (*found)->getValueByName(name) : std::numeric_limits<int64_t>::max();
}

int64_t Enum::LookupEnumNameSlow(const String &name, Enum **found) {
    auto index = LookupEnumName(name, found);
    if (index == std::numeric_limits<int64_t>::max()) {
        auto tokens = name.split(TEXT("::"));
        if (tokens.length() < 2) {
            // INVALID NAME
            return std::numeric_limits<int64_t>::max();
        }

        String enumName = tokens[tokens.length() - 2];
        for (auto pair : AllEnumByNameMap) {
            if (pair.key.contains(enumName)) {
                *found = pair.value;
                break;
            }
        }

        index = (*found != nullptr) ? (*found)->getValueByName(name) : std::numeric_limits<int64_t>::max();
    }

    return index;
}

int64_t Enum::ParseEnum(const String &str) {
    // TODO: check is valid enum string

    return LookupEnumName(str);
}

Enum *Enum::FindByName(const TCHAR *name) {
    Enum *result;
    if (LookupEnumName(name, &result)) {
        return result;
    }

    return nullptr;
}

bool Enum::setEntries(TMap<String, int64_t> &entry) {
    mEntryList = entry;

    syncWithGlobal();

    return true;
}

int64_t Enum::getValueByName(const String &name) const {
    String entryName = name.split(TEXT("::")).top();

    for (auto pair : mEntryList) {
        auto name = pair.key.split(TEXT("::")).top();
        if (name == entryName) {
            return pair.value;
        }
    }

    return (int64_t) -1;
}

const String &Enum::getNameByValue(int64_t value) const {
    for (auto &pair : mEntryList) {
        if (pair.value == value) {
            return pair.key;
        }
    }

    return String::Empty;
}

int64_t Enum::getValueByIndex(int32_t index) const {
    assert(isValidIndex(index));
    return mEntryList[index];
}

bool Enum::isValidName(const String &name) const {
    auto list = name.split(TEXT("::"));
    if (list.length() > 1) {
        String enumName = name.split(TEXT("::"))[0];

        if (!enumName.equals(getName())) {
            return false;
        }
    }

    String entryName = list.top();
    for (auto pair : mEntryList) {

        auto name = pair.key.split(TEXT("::")).top();
        if (name == entryName) {
            return true;
        }
    }

    return false;
}

bool Enum::isValidIndex(int32_t index) const {
    return 0 <= index && index < mEntryList.length();
}

bool Enum::isValidEnumValue(int64_t value) const {
    for (auto pair : mEntryList) {
        if (pair.value == value) {
            return true;
        }
    }

    return false;
}

void Enum::syncWithGlobal() {
    for (auto pair : mEntryList) {
        Enum **it = AllEnumByNameMap.find(pair.key);
        if (it == nullptr) {
            AllEnumByNameMap.add(pair.key, this);
        } else if ((*it) != this) {
            LOG(LogReflection, Warning, TEXT("Enum name is duplicated: '%s' is in both '%s', '%s'"),
                *(pair.key), TEXT("__TODO__"), TEXT("__TODO__"));
        }
    }
}