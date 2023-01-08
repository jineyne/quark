#include "Enum.h"

#include "Reflection.h"

TMap<FString, QEnum *> QEnum::AllEnumByNameMap;

QEnum::QEnum(QClass *myClass, const FString &name)
        : QField(myClass, name) {}

bool QEnum::LookupEnum(const FString &name, QEnum **found) {
    for (auto pair : AllEnumByNameMap) {
        if (pair.first.contains(name)) {
            *found = pair.second;
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
            if (pair.first.contains(enumName)) {
                *found = pair.second;
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

bool QEnum::setEntries(TArray<std::pair<FString, int64_t>> &entry) {
    mEntryList = entry;

    syncWithGlobal();

    return true;
}

int64_t QEnum::getValueByName(const FString &name) const {
    FString entryName = name.split(TEXT("::")).top();

    for (auto pair : mEntryList) {
        auto name = pair.first.split(TEXT("::")).top();
        if (name == entryName) {
            return pair.second;
        }
    }

    return (int64_t) -1;
}

const FString &QEnum::getNameByValue(int64_t value) const {
    for (auto &pair : mEntryList) {
        if (pair.second == value) {
            return pair.first;
        }
    }

    return FString::Empty;
}

int64_t QEnum::getValueByIndex(int32_t index) const {
    assert(isValidIndex(index));
    return mEntryList[index].second;
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

        auto name = pair.first.split(TEXT("::")).top();
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
        if (pair.second == value) {
            return true;
        }
    }

    return false;
}

void QEnum::syncWithGlobal() {
    for (auto pair : mEntryList) {
        QEnum **it = AllEnumByNameMap.find(pair.first);
        if (it == nullptr) {
            AllEnumByNameMap.add(pair.first, this);
        } else if ((*it) != this) {
            LOG(LogReflection, Warning, TEXT("QEnum name is duplicated: '%s' is in both '%s', '%s'"),
                *(pair.first), TEXT("__TODO__"), TEXT("__TODO__"));
        }
    }
}