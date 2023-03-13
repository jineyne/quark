#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Serialization/Archive.h"
#include "ContainerMacros.h"
#include "Memory/MemoryUtil.h"


template <typename T>
class TArray {
public:
    using Internal = std::vector<T>;

private:
    Internal mInternal;

public:
    TArray() = default;

    TArray(size_t initializeCount) {
        addUninitialized(initializeCount);
    }

    FORCEINLINE TArray(TArray& other) {
        copyToEmpty(other.getData(), other.length());
    }

    FORCEINLINE TArray(const TArray& other) {
        copyToEmpty(other.getData(), other.length());
    }

    TArray(std::initializer_list<T> initList) : mInternal(initList) {}
    TArray(TArray<T> &&other, size_t extraSlack = 0) {
        append(other);
    }

public:
    T& operator[](const size_t index) { return mInternal[index]; }
    const T &operator[](const size_t index) const { return mInternal[index]; }
    T *operator*() { return mInternal.data(); }
    const T *operator*() const { return mInternal.data(); }

    void operator=(TArray &&other) { /*appendToEmpty(other.getData(), other.length());*/ mInternal = other.mInternal; }
    void operator=(const TArray &other) { /*appendToEmpty(other.getData(), other.length());*/ mInternal = other.mInternal; }

public:
    void addUninitialized(size_t count) {
        checkf(count >= 0, TEXT("unable to add negative count"));

        // mInternal.resize(mInternal.size() + count);
        for (auto i = 0; i < count; i++) {
            add(T {});
        }
    }

    /**
     * add item to array
     *
     * @param item item to add
     */
    void add(T item) {
        mInternal.push_back(item);
    }

    /**
     * add raw array to this array
     *
     * @param ptr a pointer as array of element to add
     * @param len len of element in pointer
     */
    void append(const T *ptr, size_t len) {
        mInternal.insert(end(), ptr, ptr + len);
    }

    /**
     * append the array to this array
     */
    void append(const TArray<T> &other) {
        mInternal.insert(end(), other.begin(), other.end());
    }

    template <typename OtherT>
    void appendToEmpty(const OtherT otherData, size_t otherLen) {
        mInternal.clear();
        mInternal.reserve(otherLen);

        append(otherData, otherLen);
    }

    /**
     * clear all elements
     */
    void clear() {
        mInternal.clear();
    }

    /**
     * check if this array contains the item
     *
     * @return true if contains or false.
     */
    bool contains(T item) {
        if (std::find(begin(), end(), item) != end()) {
            return true;
        }

        return false;
    }


    template <typename OtherT>
    void copyToEmpty(const OtherT otherData, size_t otherLen) {
        mInternal.clear();
        mInternal.reserve(otherLen);
        // addUninitialized(otherLen);

        // std::memcpy(getData(), otherData, otherLen * sizeof(T));
        for (auto i = 0; i < otherLen; i++) {
            add(otherData[i]);
        }
    }

    bool empty() const {
        return mInternal.empty();
    }

    /**
     * find element within the array
     *
     * @param item item to find
     * @return gIBO of the found item or INDEX_NONE if not found
     */
    size_t find(T item) {
        size_t result = -1;
        if (find(item, result)) {
            return result;
        }

        return INDEX_NONE;
    }

    template<class Predicate>
    T *findIf(Predicate pred) {
        size_t result = -1;
        if (findIf(pred, result)) {
            return &mInternal[result];
        }

        return nullptr;
    }

    template<class Predicate>
    const T *findIf(Predicate pred) const  {
        size_t result = -1;
        if (findIf(pred, result)) {
            return &mInternal[result];
        }

        return nullptr;
    }

    /**
     * find element within the array
     *
     * @param item item to find
     * @param index gIBO of the found item
     *
     * @return ture if found or false
     */
    bool find(T item, size_t &index) {
        for(size_t i = 0; i < length(); i++) {
            if (mInternal[i] == item) {
                index = i;
                return true;
            }
        }

        return false;
    }

    /**
     * find element within the array with pred
     *
     * @param pred predicate
     * @param index gIBO of the found item
     *
     * @return ture if found or false
     */
    template<class Predicate>
    bool findIf(Predicate pred, size_t &index) {
        for(size_t i = 0; i < length(); i++) {
            if (pred(mInternal[i])) {
                index = i;
                return true;
            }
        }

        return false;
    }

    template<class Predicate>
    bool findIf(Predicate pred, size_t &index) const {
        for(size_t i = 0; i < length(); i++) {
            if (pred(mInternal[i])) {
                index = i;
                return true;
            }
        }

        return false;
    }

    /**
     * find element within the array with pred
     *
     * @param pred predicate
     * @param index gIBO of the found item
     *
     * @return ture if found or false
     */
    template<class Predicate>
    bool findEndIf(Predicate pred, size_t &index) {
        for(int64_t i = length() - 2; i > 0; --i) {
            if (pred(mInternal[i])) {
                index = i;
                return true;
            }
        }

        return false;
    }

    /**
     * return pointer to the first array entry
     */
    T *getData() {
        return mInternal.data();
    }

    /**
     * return pointer to the first array entry
     */
    const T *getData() const {
        return mInternal.data();
    }

    /**
     * set the size of the array, filling it with the given element
     */
    void init(const T &item, size_t num) {
        for (size_t i = 0; i < num; i++) {
            add(item);
        }
    }

    size_t insert(const T *ptr, size_t count, size_t index) {
        check(mInternal.length() > index);

        mInternal.insert(mInternal.begin() + index, ptr, ptr + count);
        return index;
    }

    size_t insert(const TArray<T> &other, size_t index) {
        check(mInternal.size() > index);

        mInternal.insert(mInternal.begin() + index, other.begin(), other.end());
        return index;
    }

    size_t insert(const T &item, size_t index) {
        check(mInternal.size() > index);

        mInternal.insert(mInternal.begin() + index, item);
        return index;
    }

    T pop() {
        check(mInternal.size() > 0);
        auto back = mInternal.back();
        mInternal.pop_back();
        return back;
    }

    void push(const T &item) {
        mInternal.push_back(item);
    }

    template <typename U>
    TArray<U> map(std::function<U(T)> callback) const {
        TArray<U> result(length());
        for (auto i = 0; i < length(); i++) {
            result[i] = callback(mInternal[i]);
        }

        return result;
    }

    template <typename U>
    TArray<U> map(std::function<U(T, size_t)> callback) {
        TArray<U> result(length());
        for (auto i = 0; i < length(); i++) {
            result[i] = callback(mInternal[i], i);
        }

        return result;
    }

    /**
     * remove item in array
     */
    void remove(T item) {
        mInternal.erase(std::remove(begin(), end(), item), end());
    }

    /**
     * remove at gIBO
     *
     * @param index gIBO in array of element to remove
     */
    void removeAt(size_t index) {
        mInternal.erase(mInternal.begin() + index);
    }

    /**
     * remove range
     */
    void removeRange(size_t begin, size_t end) {
        mInternal.erase(mInternal.begin() + begin, mInternal.begin() + end);
    }

    /**
     * swap with other array
     */
    void swap(TArray &other) {
        mInternal.swap(other.mInternal);
    }

    T &top() {
        return mInternal.back();
    }

    const T &top() const {
        return mInternal.back();
    }

    /**
     * reserves memory
     */
    void reserve(size_t length) {
        check(length > 0);
        mInternal.reserve(length);
    }

    void resize(size_t length) {
        check(length > 0);
        mInternal.resize(length);
    }

    void resize(size_t length, T initial) {
        check(length > 0);
        mInternal.resize(length, initial);
    }

    /**
     * returns length of elements in array
     */
    size_t length() const {
        return mInternal.size();
    }

    /**
     * return capacity of elements in array
     */
    size_t maxLength() const {
        return mInternal.capacity();
    }

    typename Internal::iterator begin() { return mInternal.begin(); }
    typename Internal::const_iterator begin() const { return mInternal.begin(); }

    typename Internal::iterator end() { return mInternal.end(); }
    typename Internal::const_iterator end() const { return mInternal.end(); }

    typename Internal::reverse_iterator rbegin() { return mInternal.rbegin(); }
    typename Internal::const_reverse_iterator rbegin() const { return mInternal.rbegin(); }

    typename Internal::reverse_iterator rend() { return mInternal.rend(); }
    typename Internal::const_reverse_iterator rend() const { return mInternal.rend(); }
};
