#pragma once

#include "CorePrerequisites.h"
#include "Image/Color.h"
#include "RenderAPI/Buffer.h"

enum class EPixelFormat {
    R8,
    RG8,
    RGB8,
    RGBA8,
    BGR8,
    BGRA8,

    R16F,
    RG16F,
    RGB16F,
    RGBA16F,

    R32F,
    RG32F,
    RGB32F,
    RGBA32F,

    D32_S8X24,
    D32_S24S8,
    D32,
    D16,

    Count,
    Unknown,
};

enum class EPixelFormatFlags {
    HasAlpha = 0x01,
    Compressed = 0x02,
    Float = 0x04,
    Depth = 0x08,
    Integer = 0x10,
    Signed = 0x20,
    Normalized = 0x40
};

ENUM_CLASS_FLAGS(EPixelFormatFlags);

enum class EPixelComponentType {
    Byte = 0,
    Short,
    Int,
    Float16,
    Float32,
    PackedR11G11B10,
    PackedR10G10B10A2,
    Count,
};

enum class ETextureType {
    e2D,
};

enum class ETextureUsage {
    Static = static_cast<uint32_t>(EBufferUsage::Static),

    Dynamic = static_cast<uint32_t>(EBufferUsage::Dynamic),

    RenderTarget = 0x200,

    DepthStencil = 0x400,

    LoadStore = 0x800,

    CpuCached = 0x1000,

    CpuReadable = 0x2000,

    MutableFormat = 0x4000,

    Default = static_cast<uint32_t>(ETextureUsage::Static),
};

ENUM_CLASS_FLAGS(ETextureUsage);

class DLL_EXPORT FPixelUtil {
public:
    static FString GetFormatName(EPixelFormat format);
    static uint32_t GetNumElemBytes(EPixelFormat format);
    static uint32_t GetBlockSize(EPixelFormat format);
    static uint32_t GetMemorySize(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format);
    static void GetSizeForMipLevel(uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevel,
                                   uint32_t &mipWidth, uint32_t &mipHeight, uint32_t &mipDepth);
    static void GetPitch(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format,
                         uint32_t &rowPitch, uint32_t &depthPitch);
    static EPixelFormatFlags GetFlags(EPixelFormat format);
    static bool HasAlpha(EPixelFormat format);
    static bool IsFloatingPoint(EPixelFormat format);
    static bool IsCompressed(EPixelFormat format);
    static bool IsNormalized(EPixelFormat format);
    static bool IsDepth(EPixelFormat format);
    static bool CheckFormat(EPixelFormat &format, ETextureType type, ETextureUsage usage);
    static uint32_t GetMaxMipmaps(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format);
    static void PackColor(const FColor &color, EPixelFormat format, void *dest);
    static void PackColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a, EPixelFormat format, void *dest);
    static void PackColor(float r, float g, float b, float a, const EPixelFormat format, void* dest);
    static void UnpackColor(FColor *color, EPixelFormat format, const void *src);
    static void UnpackColor(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a, EPixelFormat format, const void *src);
    static void UnpackColor(float *r, float *g, float *b, float *a, EPixelFormat format, const void *src);
};