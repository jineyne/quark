#pragma once

enum class EPropertyGenFlags {
    None = 0x00,
    Byte,
    Int8,
    Int32,
    Int64,
    UInt8,
    UInt32,
    UInt64,
    Float,
    Double,
    Bool,
    String,
    Class,
    Resource,
    Object, // -> pointer object
    Struct,
    NativeArray, // -> mValue[]
    Array,
    Map,
    Set,
};

enum EFieldFlags {
    FieldFlags_AccessMask   = 3,        // 0011
    FieldFlags_Public       = 0,        // 0000
    FieldFlags_Protected    = 1,        // 0001
    FieldFlags_Private      = 2,        // 0010

    FieldFlags_Pointer      = 1 << 2,   // 0100
};

enum EObjectFlags {
    ObjectFlag_None = 0,
};

enum EStructFlags {
    StructFlags_None = 0,
};

enum EClassFlags {
    ClassFlags_None = 0,
};

enum EPropertyFlags {
    PropertyFlags_None = 0,
    PropertyFlags_AccessBitMask = 0x07,
    PropertyFlags_Public = 0x01,
    PropertyFlags_Protected = 0x02,
    PropertyFlags_Private = 0x04,

    PropertyFlags_NonSerialized = 1 << 3,
    PropertyFlags_Pointer = 1 << 4,

};