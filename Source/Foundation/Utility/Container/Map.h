#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include <utility>

template <typename KeyType, typename ValueType>
class TMap {
public:
    using Internal = std::unordered_map<KeyType, ValueType>;

private:
    Internal mInternal;

public:
    TMap() = default;

    TMap(std::initializer_list<std::pair<KeyType, ValueType>> initList) {
        for (auto &it : initList) {
            mInternal.insert(std::make_pair(it.first, it.second));
        }
    }

public:
    ValueType &operator[](const KeyType &key) {
        return mInternal[key];
    }

    const ValueType &operator[](const KeyType &key) const {
        return mInternal.at(key);
    }

    TMap &operator=(std::initializer_list<std::pair<KeyType, ValueType>> initList) {
        mInternal.clear();
        for (auto &it : initList) {
            KeyType key = it.first;
            ValueType value = it.value;

            mInternal.insert(std::make_pair(key, value));
        }

        return *this;
    }

public:
    ValueType &add(KeyType key, ValueType value) {
        mInternal[key] = value;
        return mInternal[key];
    }

    ValueType &add(KeyType &key, ValueType &value) {
        mInternal[key] = value;
        return mInternal[key];
    }

    bool contains(const KeyType &key) const {
        return std::find_if(mInternal.begin(), mInternal.end(), [&](auto it) { return it.first == key; }) != mInternal.end();
    }

    ValueType *find(const KeyType &key) {
        auto it = std::find_if(mInternal.begin(), mInternal.end(), [&](auto it) { return it.first == key; });
        if (it == end()) {
            return nullptr;
        }

        return &(it->second);
    }

    const ValueType *find(const KeyType &key) const {
        const auto it = std::find_if(mInternal.begin(), mInternal.end(), [&](auto it) { return it.first == key; });
        if (it == end()) {
            return nullptr;
        }

        return &(it->second);
    }

    void remove(const KeyType &key) {
        mInternal.erase(key);
    }

    size_t length() const {
        return mInternal.size();
    }

    typename Internal::iterator begin() { return mInternal.begin(); }
    typename Internal::const_iterator begin() const { return mInternal.begin(); }

    typename Internal::iterator end() { return mInternal.end(); }
    typename Internal::const_iterator end() const { return mInternal.end(); }
};
