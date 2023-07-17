#pragma once

#include "CorePrerequisites.h"
#include "Image/Color.h"
#include "RenderAPI/Buffer.h"
#include "PixelUtil.g.h"

QENUM()
enum class EPixelFormat {
    R8                                                          QENTRY(),
    RG8                                                         QENTRY(),
    RGB8                                                        QENTRY(),
    RGBA8                                                       QENTRY(),
    BGR8                                                        QENTRY(),
    BGRA8                                                       QENTRY(),

    R16F                                                        QENTRY(),
    RG16F                                                       QENTRY(),
    RGB16F                                                      QENTRY(),
    RGBA16F                                                     QENTRY(),

    R32F                                                        QENTRY(),
    RG32F                                                       QENTRY(),
    RGB32F                                                      QENTRY(),
    RGBA32F                                                     QENTRY(),

    D32_S8X24                                                   QENTRY(),
    D32_S24S8                                                   QENTRY(),
    D32                                                         QENTRY(),
    D16                                                         QENTRY(),

    Count,
    Unknown                                                     QENTRY(),
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

QENUM()
enum class ETextureType {
    e2D                                                         QENTRY(),
};

QENUM()
enum class ETextureUsage {
    Static = static_cast<uint32_t>(EBufferUsage::Static)        QENTRY(),
    Dynamic = static_cast<uint32_t>(EBufferUsage::Dynamic)      QENTRY(),

    RenderTarget = 0x200                                        QENTRY(),
    DepthStencil = 0x400                                        QENTRY(),
    LoadStore = 0x800                                           QENTRY(),
    CpuCached = 0x1000                                          QENTRY(),
    CpuReadable = 0x2000                                        QENTRY(),
    MutableFormat = 0x4000                                      QENTRY(),
    Default = static_cast<uint32_t>(ETextureUsage::Static)      QENTRY(),
};

ENUM_CLASS_FLAGS(ETextureUsage);

class DLL_EXPORT PixelUtil {
public:
    static String GetFormatName(EPixelFormat format);
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
    static void PackColor(const Color &color, EPixelFormat format, void *dest);
    static void PackColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a, EPixelFormat format, void *dest);
    static void PackColor(float r, float g, float b, float a, const EPixelFormat format, void* dest);
    static void UnpackColor(Color *color, EPixelFormat format, const void *src);
    static void UnpackColor(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a, EPixelFormat format, const void *src);
    static void UnpackColor(float *r, float *g, float *b, float *a, EPixelFormat format, const void *src);
};