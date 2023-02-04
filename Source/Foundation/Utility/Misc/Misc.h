#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

struct EnumClassHash {
    template <typename T>
    constexpr std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

template<class T>
void CombineHash(size_t &seed, const T &v) {
    using HashType = typename std::conditional<std::is_enum<T>::value, EnumClassHash, std::hash<T>>::type;

    HashType hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}