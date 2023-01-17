#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

template <typename T>
void memzero(T &data) {
    memset(&data, 0, sizeof(T));
}

template <typename T>
void memzero(T &data, size_t size) {
    memset(&data, 0, size);
}

template <typename T>
struct TIsTriviallyCopyConstructible {
    enum { Value = __has_trivial_copy(T) };
};

template <typename T, typename Arg>
struct TIsBitwiseConstructible {
    // Assume no bitwise construction in general
    enum { Value = false };
};

template <typename T>
struct TIsBitwiseConstructible<T, T> {
    // Ts can always be bitwise constructed from itself if it is trivially copyable.
    enum { Value = TIsTriviallyCopyConstructible<T>::Value };
};

template <typename T, typename U>
struct TIsBitwiseConstructible<const T, U> : TIsBitwiseConstructible<T, U> {
    // Constructing a const T is the same as constructing a T
};

// Const pointers can be bitwise constructed from non-const pointers.
// This is not true for pointer conversions in general, e.g. where an offset may need to be applied in the case
// of multiple inheritance, but there is no way of detecting that at compile-time.
template <typename T>
struct TIsBitwiseConstructible<const T*, T*> {
    // Constructing a const T is the same as constructing a T
    enum { Value = true };
};

// Unsigned types can be bitwise converted to their signed equivalents, and vice versa.
// (assuming two's-complement, which we are)
template <> struct TIsBitwiseConstructible<uint8_t,   int8_t>  { enum { Value = true }; };
template <> struct TIsBitwiseConstructible< int8_t,  uint8_t>  { enum { Value = true }; };
template <> struct TIsBitwiseConstructible<uint16_t,  int16_t> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible< int16_t, uint16_t> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible<uint32_t,  int32_t> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible< int32_t, uint32_t> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible<uint64_t,  int64_t> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible< int64_t, uint64_t> { enum { Value = true }; };


template <typename DestinationElementType, typename SourceElementType, typename SizeType>
FORCEINLINE void constructItems(void* dest, const SourceElementType* src, SizeType length) {
    if constexpr (TIsBitwiseConstructible<DestinationElementType, SourceElementType>::Value) {
        memcpy(dest, src, length);
    } else {
        while (length) {
            new(dest) DestinationElementType(*src);
            ++(DestinationElementType * &)
            dest;
            ++src;
            --length;
        }
    }
}