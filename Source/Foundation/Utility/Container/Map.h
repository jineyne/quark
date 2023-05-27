#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Array.h"
#include <utility>

template <typename KeyType, typename ValueType>
struct TPair {
    KeyType key;
    ValueType value;

    TPair(const KeyType &key, const ValueType &value)
            : key(key), value(value) {}
};

template <typename KeyType, typename ValueType, class HashFunction = std::hash<KeyType>, class EqualFunction = std::equal_to<KeyType>, class Allocator = StdAllocator<TPair<KeyType, ValueType>>, int InitialCapacity = 16>
struct TMap {
public:
    using ListType = std::list<TPair<KeyType, ValueType>, Allocator>;
    using TableType = TArray<ListType>;

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = TPair<KeyType, ValueType>;
        using difference_type = std::ptrdiff_t;
        using pointer = TPair<KeyType, ValueType> *;
        using reference = TPair<KeyType, ValueType> &;

        iterator() = default;

        iterator(TableType *table, size_t index, typename ListType::iterator listIt)
                : table(table), index(index), listIt(listIt) {
            advancePastEmptyBuckets();
        }

        reference operator*() const {
            return *listIt;
        }

        pointer operator->() const {
            return &*listIt;
        }

        iterator &operator++() {
            ++listIt;
            advancePastEmptyBuckets();
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const iterator &other) const {
            return table == other.table && index == other.index && listIt == other.listIt;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }

    private:
        void advancePastEmptyBuckets() {
            while (index < table->length() && listIt == (*table)[index].end()) {
                ++index;
                if (index < table->length()) {
                    listIt = (*table)[index].begin();
                }
            }
        }

        TableType *table = nullptr;
        size_t index = 0;
        typename ListType::iterator listIt;
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = TPair<KeyType, ValueType>;
        using difference_type = std::ptrdiff_t;
        using pointer = const TPair<KeyType, ValueType> *;
        using reference = const TPair<KeyType, ValueType> &;

        const_iterator() = default;

        const_iterator(const TableType *table, size_t index, typename ListType::const_iterator listIt)
                : table(table), index(index), listIt(listIt) {
            advancePastEmptyBuckets();
        }

        reference operator*() const {
            return *listIt;
        }

        pointer operator->() const {
            return &*listIt;
        }

        const_iterator &operator++() {
            ++listIt;
            advancePastEmptyBuckets();
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const const_iterator &other) const {
            return table == other.table && index == other.index && listIt == other.listIt;
        }

        bool operator!=(const const_iterator &other) const {
            return !(*this == other);
        }

    private:
        void advancePastEmptyBuckets() {
            while (index < table->length() && listIt == (*table)[index].end()) {
                ++index;
                if (index < table->length()) {
                    listIt = (*table)[index].begin();
                }
            }
        }

        const TableType *table = nullptr;
        size_t index = 0;
        typename ListType::const_iterator listIt;
    };

private:
    TableType mTable;

    HashFunction mHashFunc;
    EqualFunction mEqualFunc;

    size_t mCapacity;
    size_t mLength;

    double mLoadFactor = 0.75;

public:
    TMap(size_t capacity = InitialCapacity) : mCapacity(capacity), mLength(0) {
        mTable.addUninitialized(mCapacity);
    }

    TMap(std::initializer_list<TPair<KeyType, ValueType>> initList) : mCapacity(InitialCapacity), mLength(0) {
        mTable.addUninitialized(mCapacity);

        for (auto &entry : initList) {
            add(entry.key, entry.value);
        }
    }

public:
    ValueType &operator[](const KeyType &key) {
        return at(key);
    }

    const ValueType &operator[](const KeyType &key) const {
        return *find(key);
    }

    TMap &operator=(std::initializer_list<TPair<KeyType, ValueType>> initList) {
        reset();
        for (const auto &entry : initList) {
            add(entry.first, entry.second);
        }
        return *this;
    }

public:
    ValueType &add(KeyType key, ValueType value) {
        size_t index = mHashFunc(key) % mCapacity;
        for (auto &entry : mTable[index]) {
            if (mEqualFunc(entry.key, key)) {
                entry.value = value;
                return entry.value;
            }
        }
        TPair newPair = {key, value};
        mTable[index].push_back(newPair);
        mLength++;

        if (mLength >= mCapacity * mLoadFactor) {
            rehash(mCapacity * 2);
            return mTable[mHashFunc(key) % mCapacity].back().value;
        }

        return mTable[index].back().value;
    }

    ValueType &at(KeyType key) {
        ValueType *value = find(key);
        if (value) {
            return *value;
        } else {
            return add(key, ValueType{});
        }
    }

    /*ValueType &add(KeyType &key, ValueType &mValue) {
        mTable[key] = mValue;
        return mTable[key];
    }*/

    bool contains(const KeyType &key) const {
        size_t index = mHashFunc(key) % mCapacity;
        for (const auto &entry : mTable[index]) {
            if (mEqualFunc(entry.key, key)) {
                return true;
            }
        }
        return false;
    }

    bool empty() const {
        return mLength == 0;
    }

    ValueType *find(const KeyType &key) {
        size_t index = mHashFunc(key) % mCapacity;
        for (auto &entry : mTable[index]) {
            if (mEqualFunc(entry.key, key)) {
                return &entry.value;
            }
        }
        return nullptr;
    }

    const ValueType *find(const KeyType &key) const {
        size_t index = mHashFunc(key) % mCapacity;
        for (const auto &entry : mTable[index]) {
            if (mEqualFunc(entry.key, key)) {
                return &entry.value;
            }
        }
        return nullptr;
    }

    void reset() {
        mLength = 0;
        mCapacity = 16;

        mTable.clear();
        mTable.resize(mCapacity);
    }

    void remove(const KeyType &key) {
        size_t index = mHashFunc(key) % mCapacity;
        auto &bucket = mTable[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (mEqualFunc(it->key, key)) {
                bucket.erase(it);
                mLength--;
                return;
            }
        }
    }

    size_t length() const {
        return mLength;
    }

    iterator begin() { return iterator(&mTable, 0, mTable[0].begin()); }
    const_iterator begin() const { return const_iterator(&mTable, 0, mTable[0].begin()); }

    iterator end() { return iterator(&mTable, mTable.length(), mTable.top().end()); }
    const_iterator end() const { return const_iterator(&mTable, mTable.length(), mTable.top().end()); }

private:
    void rehash(size_t newCapacity) {
        TableType new_table(newCapacity);
        for (const auto &bucket : mTable) {
            for (const auto &entry : bucket) {
                size_t index = mHashFunc(entry.key) % newCapacity;
                new_table[index].emplace_back(entry);
            }
        }
        mTable = std::move(new_table);
        mCapacity = newCapacity;
    }

    friend class MapProperty;
};
