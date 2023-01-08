#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Traits/IntTraits.h"

template <typename CharType, const unsigned int Size>
struct TCharBase {
    static constexpr CharType LineFeed = 0xa;
    static constexpr CharType VerticalTab = 0xb;
    static constexpr CharType FormFeed = 0xc;
    static constexpr CharType CarriageReturn = 0xd;
    static constexpr CharType NextLine = 0x85;
    static constexpr CharType LineSeparator = 0x2028;
    static constexpr CharType ParagraphSeparator = 0x2029;

    static bool IsLinebreak(CharType Char) {
        return ((uint32(Char) - LineFeed) <= uint32(CarriageReturn - LineFeed)) |
               (Char == NextLine) | (Char == LineSeparator) | (Char == ParagraphSeparator);
    }

};

template <typename CharType>
struct TCharBase<CharType, 1> {
    static constexpr CharType LineFeed = 0xa;
    static constexpr CharType VerticalTab = 0xb;
    static constexpr CharType FormFeed = 0xc;
    static constexpr CharType CarriageReturn = 0xd;

    static bool IsLinebreak(CharType Char) {
        return ((uint32(Char) - LineFeed) <= uint32(CarriageReturn - LineFeed));
    }
};

template <typename CharType>
struct TChar : TCharBase<CharType, sizeof(CharType)> {
    static constexpr FORCEINLINE uint32_t ToUnsigned(CharType Char) {
        return (typename TUnsignedIntType<sizeof(CharType)>::Type) Char;
    }

    /**
	* Only converts ASCII characters, same as CRT to[w]upper() with standard C locale
	*/
    static CharType ToUpper(CharType Char) {
        return (CharType) (ToUnsigned(Char) - ((uint32_t(Char) - 'a' < 26u) << 5));
    }

    /**
    * Only converts ASCII characters, same as CRT to[w]upper() with standard C locale
    */
    static CharType ToLower(CharType Char) {
        return (CharType) (ToUnsigned(Char) + ((uint32_t(Char) - 'A' < 26u) << 5));
    }
};

using FChar = TChar<TCHAR>;
