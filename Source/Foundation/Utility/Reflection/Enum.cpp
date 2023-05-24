#include "Enum.h"

#include "Reflection.h"

TMap<FString, QEnum *> QEnum::AllEnumByNameMap;

QEnum::QEnum(QClass *myClass, const FString &name)
        : QField(myClass, name) {}

bool QEnum::LookupEnum(const FString &name, QEnum **found) {
    for (auto pair : AllEnumByNameMap) {
        if (pair.key.contains(name)) {
            *found = pair.value;
            return true;
        }
    }

    return false;
}

int64_t QEnum::LookupEnumName(const FString &name, QEnum **found) {
    auto it = AllEnumByNameMap.find(name);
    if (it == nullptr) {
        return std::numeric_limits<int64_t>::max();
    }

    *found = *it;
    return (*found != nullptr) ? (*found)->getValueByName(name) : std::numeric_limits<int64_t>::max();
}

int64_t QEnum::LookupEnumNameSlow(const FString &name, QEnum **found) {
    auto index = LookupEnumName(name, found);
    if (index == std::numeric_limits<int64_t>::max()) {
        auto tokens = name.split(TEXT("::"));
        if (tokens.length() < 2) {
            // INVALID NAME
            return std::numeric_limits<int64_t>::max();
        }

        FString enumName = tokens[tokens.length() - 2];
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

int64_t QEnum::ParseEnum(const FString &str) {
    // TODO: check is valid enum string

    return LookupEnumName(str);
}

bool QEnum::setEntries(TMap<FString, int64_t> &entry) {
    mEntryList = entry;

    syncWithGlobal();

    return true;
}

int64_t QEnum::getValueByName(const FString &name) const {
    FString entryName = name.split(TEXT("::")).top();

    for (auto pair : mEntryList) {
        auto name = pair.key.split(TEXT("::")).top();
        if (name == entryName) {
            return pair.value;
        }
    }

    return (int64_t) -1;
}

const FString &QEnum::getNameByValue(int64_t value) const {
    for (auto &pair : mEntryList) {
        if (pair.value == value) {
            return pair.key;
        }
    }

    return FString::Empty;
}

int64_t QEnum::getValueByIndex(int32_t index) const {
    assert(isValidIndex(index));
    return mEntryList[index];
}

bool QEnum::isValidName(const FString &name) const {
    auto list = name.split(TEXT("::"));
    if (list.length() > 1) {
        FString enumName = name.split(TEXT("::"))[0];

        if (!enumName.equals(getName())) {
            return false;
        }
    }

    FString entryName = list.top();
    for (auto pair : mEntryList) {

        auto name = pair.key.split(TEXT("::")).top();
        if (name == entryName) {
            return true;
        }
    }

    return false;
}

bool QEnum::isValidIndex(int32_t index) const {
    return 0 <= index && index < mEntryList.length();
}

bool QEnum::isValidEnumValue(int64_t value) const {
    for (auto pair : mEntryList) {
        if (pair.value == value) {
            return true;
        }
    }

    return false;
}

void QEnum::syncWithGlobal() {
    for (auto pair : mEntryList) {
        QEnum **it = AllEnumByNameMap.find(pair.key);
        if (it == nullptr) {
            AllEnumByNameMap.add(pair.key, this);
        } else if ((*it) != this) {
            LOG(LogReflection, Warning, TEXT("QEnum name is duplicated: '%s' is in both '%s', '%s'"),
                *(pair.key), TEXT("__TODO__"), TEXT("__TODO__"));
        }
    }
}