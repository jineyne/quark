#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

#include "Memory/InlineAllocator.h"
#include "Misc/CString.h"

#define DEFAULT_STRING_CONVERSION_SIZE 128u
#define UNICODE_BOGUS_CHAR_CODEPOINT '?'

class FStringConverter {
public:
    /**
	 * Metafunction which tests whether a given character type represents a fixed-width encoding.
	 *
	 * We need to 'forward' the metafunction to a helper because of the C++ requirement that
	 * nested structs cannot be fully specialized.  They can be partially specialized however, hence the
	 * helper function.
	 */
    template <bool Dummy, typename T>
    struct TIsFixedWidthEncoding_Helper {
        enum { Value = false };
    };

    template <bool Dummy> struct TIsFixedWidthEncoding_Helper<Dummy, ANSICHAR> { enum { Value = true }; };
    template <bool Dummy> struct TIsFixedWidthEncoding_Helper<Dummy, WIDECHAR> { enum { Value = true }; };

    template <typename T>
    struct TIsFixedWidthEncoding : TIsFixedWidthEncoding_Helper<false, T> {
    };

    /**
	 * Metafunction which tests whether two encodings are compatible.
	 *
	 * We'll say the encodings are compatible if they're both fixed-width and have the same size.  This
	 * should be good enough and catches things like UCS2CHAR and WIDECHAR being equivalent.
	 * Specializations of this template can be provided for any other special cases.
	 * Same size is a minimum requirement.
	 */
    template <typename EncodingA, typename EncodingB>
    struct TAreEncodingsCompatible {
        enum { Value = TIsFixedWidthEncoding<EncodingA>::Value && TIsFixedWidthEncoding<EncodingB>::Value && sizeof(EncodingA) == sizeof(EncodingB) };
    };

    template <typename Encoding>
    static constexpr bool IsFixedWidthEncoding() {
        return std::is_same_v<Encoding, ANSICHAR> ||
               std::is_same_v<Encoding, WIDECHAR>;
    }

    template <typename SrcEncoding, typename DstEncoding>
    static constexpr bool IsCharEncodingCompatibleWith() {
        if constexpr (std::is_same_v<SrcEncoding, DstEncoding>) {
            return true;
        }
#if PLATFORM_TCHAR_IS_CHAR16
        else if constexpr (std::is_same_v<SrcEncoding, WIDECHAR> && std::is_same_v<DstEncoding, wchar_t>) {
			return true;
		} else if constexpr (std::is_same_v<SrcEncoding, wchar_t> && std::is_same_v<DstEncoding, WIDECHAR>) {
			return true;
		}
#endif

        return false;
    }


    template <typename SrcEncoding, typename DstEncoding>
    static constexpr bool IsCharEncodingSimplyConvertibleTo() {
        if constexpr (IsCharEncodingCompatibleWith<SrcEncoding, DstEncoding>()) {
            return true;
        } else if (IsFixedWidthEncoding<SrcEncoding>() && sizeof(DstEncoding) >= sizeof(SrcEncoding)) {
            return true;
        } else {
            return false;
        }
    }

    template <typename DstEncoding, typename SrcEncoding>
    static constexpr bool CanConvertCodepoint(SrcEncoding cp) {
        static_assert(IsFixedWidthEncoding<SrcEncoding>(), "Source encoding is not fixed-width");

        if constexpr (IsCharEncodingCompatibleWith<SrcEncoding, DstEncoding>()) {
            return true;
        } else if constexpr (!IsFixedWidthEncoding<DstEncoding>()) {
            return true;
        } else if constexpr (std::is_same_v<DstEncoding, ANSICHAR>) {
            return (uint32_t) cp <= 0x7f;
        } else {
            static_assert(sizeof(SrcEncoding) == 4 && sizeof(DstEncoding) == 2, "Unimplemented conversion");

            // Can't encode more than 16-bit in UCS-2
            return (uint32_t) cp <= 0xffff;
        }
    }

    template <typename SrcEncoding, typename DstEncoding>
    static FORCEINLINE DstEncoding *Convert(DstEncoding *dst, int32_t dstSize, const SrcEncoding *src, int32_t srcSize) {
        if constexpr (IsCharEncodingCompatibleWith<SrcEncoding, DstEncoding>()) {
            if (dstSize < srcSize) {
                return nullptr;
            }

            return (DstEncoding *) memcpy(dst, src, srcSize * sizeof(SrcEncoding)) + srcSize;
        } else if constexpr (IsCharEncodingSimplyConvertibleTo<SrcEncoding, DstEncoding>()) {
            const int32_t size = dstSize <= srcSize ? dstSize : srcSize;
            for (int32_t i = 0; i < size; ++i) {
                SrcEncoding ch = src[i];
                dst[i] = (DstEncoding) ch;
            }

            return (dstSize < srcSize) ? nullptr : dst + size;
        }  else if constexpr (IsFixedWidthEncoding<SrcEncoding>() && IsFixedWidthEncoding<DstEncoding>()) {
            const int32_t size = dstSize <= srcSize ? dstSize : srcSize;
            bool bInvalidChars = false;
            for (int i = 0; i < size; ++i) {
                SrcEncoding ch = src[i];
                dst[i] = (DstEncoding) ch;

                bInvalidChars |= !CanConvertCodepoint<DstEncoding>(ch);
            }

            if (bInvalidChars) {
                for (int i = 0; i < size; ++i) {
                    if (!CanConvertCodepoint<DstEncoding>(src[i])) {
                        dst[i] = UNICODE_BOGUS_CHAR_CODEPOINT;
                    }
                }
            }

            return dstSize < srcSize ? nullptr : dst + size;
        }

        return nullptr;
    }

    template <typename DstEncoding, typename SrcEncoding>
    static int32_t ConvertedLength(const SrcEncoding *src, int32_t srcLen) {
        if constexpr (IsCharEncodingSimplyConvertibleTo<SrcEncoding, DstEncoding>() ||
                      (IsFixedWidthEncoding<SrcEncoding>() && IsFixedWidthEncoding<DstEncoding>())) {
            return srcLen;
        }

        return -1;
    }

};

template<typename InFromType, typename InToType = InFromType>
class TStringConverter {
public:
    typedef InFromType FromType;
    typedef InToType   ToType;

    template <
            typename CharType,
            std::enable_if_t<FStringConverter::IsCharEncodingCompatibleWith<CharType, FromType>()>* = nullptr
    >
    FORCEINLINE static void Convert(ToType* dest, int32_t destLen, const CharType* src, int32_t srcLen) {
        ToType* Result = FStringConverter::Convert(dest, destLen, (const FromType *) src, srcLen);
        check(Result);
    }

    template <
            typename CharType,
            std::enable_if_t<FStringConverter::IsCharEncodingCompatibleWith<CharType, FromType>()>* = nullptr
    >
    static int32_t ConvertedLength(const CharType* src, int32_t srcLen) {
        return FStringConverter::ConvertedLength<ToType>((const FromType *) src, srcLen);
    }
};

/**
 * Class takes one type of string and and stores it as-is.
 * For API compatibility with TStringConversion when the To and From types are already in the same format.
 */
template<typename InFromType, typename InToType = InFromType>
class TStringPointer {
    static_assert(sizeof(InFromType) == sizeof(InToType), "FromType must be the same size as ToType!");

public:
    typedef InFromType FromType;
    typedef InToType   ToType;

public:
    template <
            typename SrcBufferType,
            std::enable_if_t<FStringConverter::IsCharEncodingCompatibleWith<SrcBufferType, FromType>()>* = nullptr
    >
    explicit TStringPointer(const SrcBufferType* Source) {
        if (Source) {
            mPtr = (const ToType*)Source;
            mStringLength = -1; // Length calculated on-demand
        } else {
            mPtr = nullptr;
            mStringLength = 0;
        }
    }

    template <
            typename SrcBufferType,
            std::enable_if_t<FStringConverter::IsCharEncodingCompatibleWith<SrcBufferType, FromType>()>* = nullptr
    >
    TStringPointer(const SrcBufferType* Source, int32_t SourceLen) {
        if (Source) {
            if (SourceLen > 0 && ((const FromType*)Source)[SourceLen-1] == 0) {
                // Given buffer is null-terminated
                SourceLen -= 1;
            }

            mPtr = (const ToType*)Source;
            mStringLength = SourceLen;
        } else {
            mPtr = nullptr;
            mStringLength = 0;
        }
    }

    /**
     * Move constructor
     */
    TStringPointer(TStringPointer&& Other) = default;

    /**
     * Accessor for the string.
     * @note The string may not be null-terminated if constructed from an explicitly sized buffer that didn't include the null-terminator.
     *
     * @return A const pointer to the string.
     */
    FORCEINLINE const ToType* get() const {
        return mPtr;
    }

    /**
     * Length of the string.
     *
     * @return The number of characters in the string, excluding any null terminator.
     */
    FORCEINLINE int32_t length() const {
        return mStringLength == -1 ? TCString<ToType>::Strlen(mPtr) : mStringLength;
    }

private:
    // Non-copyable
    TStringPointer(const TStringPointer&) = delete;
    TStringPointer& operator=(const TStringPointer&) = delete;

    const ToType* mPtr;
    int32_t mStringLength;
};

template <typename Converter, int DefaultSize = DEFAULT_STRING_CONVERSION_SIZE>
class TStringConversion : private Converter, private TInlineAllocator<typename Converter::ToType, DefaultSize> {
public:
    typedef typename Converter::ToType ToType;
    typedef typename Converter::FromType FromType;

    typedef class TInlineAllocator<ToType, DEFAULT_STRING_CONVERSION_SIZE> Allocator;

private:
    ToType *mPtr;
    int32_t mStringLength;

private:
    // Non-copyable
    TStringConversion(const TStringConversion&) = delete;
    TStringConversion& operator=(const TStringConversion&) = delete;

public:
    explicit TStringConversion(const FromType *source) {
        if (source) {
            init(source, TCString<FromType>::Strlen(source));
        } else {
            mPtr = nullptr;
            mStringLength = 0;
        }
    }

public:
    FORCEINLINE const ToType *get() const {
        return mPtr;
    }

    FORCEINLINE int32_t length() const {
        return mStringLength;
    }

private:
    void init(const FromType *src, int32_t srcLen) {
        mStringLength = Converter::ConvertedLength((FromType *) src, srcLen);

        int32_t bufferSize = mStringLength + 1;
        Allocator::resize(0, bufferSize, sizeof(ToType));

        mPtr = Allocator::getAllocation();
        Converter::Convert(mPtr, bufferSize, src, srcLen + 1);
    }
};

#define TCHAR_TO_ANSI(str) (ANSICHAR*)StringCast<ANSICHAR>(static_cast<const TCHAR*>(str)).get()
#define ANSI_TO_TCHAR(str) (TCHAR*)StringCast<TCHAR>(static_cast<const ANSICHAR*>(str)).get()

template <typename To, int32_t DefaultConversionSize = DEFAULT_STRING_CONVERSION_SIZE, typename From>
FORCEINLINE auto StringCast(const From *str) {
    if constexpr (FStringConverter::IsCharEncodingCompatibleWith<From, To>()) {
        return TStringPointer<To>((const To*) str);
    } else {
        return TStringConversion<TStringConverter<From, To>, DefaultConversionSize>(str);
    }
}
