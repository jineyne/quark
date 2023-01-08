#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

template <int NumBytes>
struct TSignedIntType {
};

template <>
struct TSignedIntType<1> {
    using Type = int8_t;
};
template <>
struct TSignedIntType<2> {
    using Type = int16_t;
};
template <>
struct TSignedIntType<4> {
    using Type = int32_t;
};
template <>
struct TSignedIntType<8> {
    using Type = int64_t;
};


/**
 * Type trait which yields an unsigned integer type of a given number of bytes.
 * If there is no such type, the Type member type will be absent, allowing it to be used in SFINAE contexts.
 */
template <int NumBytes>
struct TUnsignedIntType {
};

template <>
struct TUnsignedIntType<1> {
    using Type = uint8_t;
};
template <>
struct TUnsignedIntType<2> {
    using Type = uint16_t;
};
template <>
struct TUnsignedIntType<4> {
    using Type = uint32_t;
};
template <>
struct TUnsignedIntType<8> {
    using Type = uint64_t;
};

