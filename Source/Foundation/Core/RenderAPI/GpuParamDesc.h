#pragma once

#include "CorePrerequisites.h"
#include "GpuBuffer.h"

enum class EGpuParamDataType {
    Float1 = 1,
    Float2,
    Float3,
    Float4,

    Matrix2x2 = 11,
    Matrix2x3,
    Matrix2x4,
    Matrix3x2,
    Matrix3x3,
    Matrix3x4,
    Matrix4x2,
    Matrix4x3,
    Matrix4x4,

    Int1 = 20,
    Int2,
    Int3,
    Int4,

    Bool = 24,
    Struct = 25,
    Color = 26,
    Count = 27,

    Unknown,
};

enum class EGpuParamObjectType {
    Sampler1D = 1,
    Sampler2D,
    Sampler3D,

    SamplerCube,
    Sampler2DMS,

    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
    Texture2DMS,

    ByteBuffer,
    StructuredBuffer,
    RWTypedBuffer,

    Texture1DArray,
    Texture2DArray,
    TextureCubeArray,
    Texture2DMSArray,

    Unknown,
};

struct GpuParamDataDesc {
    String name;

    uint32_t elementSize;
    uint32_t arraySize;
    uint32_t arrayElementStride;

    EGpuParamDataType type;

    uint32_t paramBlockSlot;
    uint32_t paramBlockSet;
    uint32_t gpuMemOffset;
    uint32_t cpuMemOffset;
};

struct GpuParamObjectDesc {
    String name;
    EGpuParamObjectType type;

    uint32_t slot;
    uint32_t set;

    EGpuBufferFormat elementType = EGpuBufferFormat::Unknown;
};

struct GpuParamBlockDesc {
    String name;

    uint32_t slot;
    uint32_t set;

    uint32_t blockSize;

    bool isShareable;
};

struct DLL_EXPORT GpuParamDesc {
    using GpuParamBlockDescMap = TMap<String, GpuParamBlockDesc>;
    using GpuParamDataDescMap = TMap<String, GpuParamDataDesc>;
    using GpuParamObjectDescMap = TMap<String, GpuParamObjectDesc>;

    GpuParamBlockDescMap paramBlocks;
    GpuParamDataDescMap params;

    GpuParamObjectDescMap samplers;
    GpuParamObjectDescMap textures;
    GpuParamObjectDescMap loadStoreTextures;
    GpuParamObjectDescMap buffers;
};