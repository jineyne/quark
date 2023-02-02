#pragma once

#include "QHTPrerequisites.h"

enum class ETokenType {
    Directive,

    Identifier,
    Literal,

    Symbol,
    Unknown,
};

enum class ELiteralType {
    String,
    Boolean,
    UInt8,
    Int8,
    UInt32,
    Int32,
    UInt64,
    Int64,
    Float,
    Double,
    Unknown,
};

enum class EAccessControlType {
    Public,
    Private,
    Protected
};

struct FLiteral {
        ELiteralType type = ELiteralType::Unknown;
        union {
            bool b;
            uint8_t u8;
            int8_t i8;
            uint32_t u32;
            int32_t i32;
            uint64_t u64;
            int64_t i64;
            float f;
            double d;
        };
        FString s;
};

struct FToken {
        ETokenType type = ETokenType::Unknown;
        size_t pos;
        size_t line;
        FString token;

        FLiteral literal;
};