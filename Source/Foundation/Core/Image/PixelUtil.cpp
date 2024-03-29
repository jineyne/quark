#include "PixelUtil.h"
#include "Math/Bitwise.h"
#include "Resource/Resources.h"

struct PixelFormatDescription {
    String name; /**< format name. */
    uint8_t elemBytes; /**< use byte per one element (color) */
    EPixelFormatFlags flags; /**< PixelFormatFlags is declare as pixel format. */
    EPixelComponentType componentType; /**< data type for single element of component. */
    uint8_t componentCount; /**< component count. */

    uint8_t rbits, gbits, bbits, abits; /**< bit per component element. */

    uint32_t rmask, gmask, bmask, amask; /**< packers/unpackers mask. */
    uint8_t rshift, gshift, bshift, ashift; /**< packers/unpackers shift. */
};

PixelFormatDescription _pixelFormats[static_cast<uint32_t>(EPixelFormat::Count)] = {
        {
                TEXT("R8"),
                /* Bytes per element */
                1,
                /* Flags */
                EPixelFormatFlags::Integer | EPixelFormatFlags::Compressed,
                /* Component type and count */
                EPixelComponentType::Byte,    1,
                /* rbits, gbits, bbits, abits */
                8,  0,  0,  0,
                /* Masks and shifts */
                0x000000FF, 0,          0,          0,
                0, 0,  0,  0,
        },
        //-----------------------------------------------------------------------
        {
                TEXT("RG8"),
                /* Bytes per element */
                2,
                /* Flags */
                EPixelFormatFlags::Integer | EPixelFormatFlags::Normalized,
                /* Component type and count */
                EPixelComponentType::Byte,    2,
                /* rbits, gbits, bbits, abits */
                8,  8,  0,  0,
                /* Masks and shifts */
                0x000000FF, 0x0000FF00, 0,          0,
                0, 8,  0,  0,
        },
        //-----------------------------------------------------------------------
        {
                TEXT("RGB8"),
                /* Bytes per element */
                4,  // 4th byte is unused
                /* Flags */
                EPixelFormatFlags::Integer | EPixelFormatFlags::Normalized,
                /* Component type and count */
                EPixelComponentType::Byte,    3,
                /* rbits, gbits, bbits, abits */
                8,  8,  8,  0,
                /* Masks and shifts */
                0x000000FF, 0x0000FF00, 0x00FF0000, 0,
                0, 8,  16, 0,
        },
        //-----------------------------------------------------------------------
        {
                TEXT("RGBA8"),
                /* Bytes per element */
                4,
                /* Flags */
                EPixelFormatFlags::HasAlpha | EPixelFormatFlags::Integer | EPixelFormatFlags::Normalized,
                /* Component type and count */
                EPixelComponentType::Byte,    4,
                /* rbits, gbits, bbits, abits */
                8,  8,  8,  8,
                /* Masks and shifts */
                0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000,
                0, 8,  16, 24,
        },
        //-----------------------------------------------------------------------
        {
                TEXT("BGR8"),
                /* Bytes per element */
                4,  // 4th byte is unused
                /* Flags */
                EPixelFormatFlags::Integer | EPixelFormatFlags::Normalized,
                /* Component type and count */
                EPixelComponentType::Byte, 3,
                /* rbits, gbits, bbits, abits */
                8, 8, 8, 0,
                /* Masks and shifts */
                0x00FF0000, 0x0000FF00, 0x000000FF, 0,
                16, 8, 0, 0,
        },
        //-----------------------------------------------------------------------
        {
                TEXT("BGRA8"),
                /* Bytes per element */
                4,
                /* Flags */
                EPixelFormatFlags::HasAlpha | EPixelFormatFlags::Integer | EPixelFormatFlags::Normalized,
                /* Component type and count */
                EPixelComponentType::Byte, 4,
                /* rbits, gbits, bbits, abits */
                8, 8, 8, 8,
                /* Masks and shifts */
                0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000,
                16, 8, 0, 24,
        },
        //-----------------------------------------------------------------------
        {
                TEXT("RGBA16F"),
                /* Bytes per element */
                8,
                /* Flags */
                EPixelFormatFlags::Float | EPixelFormatFlags::HasAlpha,
                /* Component type and count */
                EPixelComponentType::Float16, 4,
                /* rbits, gbits, bbits, abits */
                16, 16, 16, 16,
                /* Masks and shifts */
                0x0000FFFF, 0xFFFF0000, 0x0000FFFF, 0xFFFF0000,
                0, 16, 0,  16,
        },
        //-----------------------------------------------------------------------
        {
                TEXT("Unknown"),
                /* Bytes per element */
                0,
                /* Flags */
                (EPixelFormatFlags) 0,
                /* Component type and count */
                EPixelComponentType::Byte,    0,
                /* rbits, gbits, bbits, abits */
                0,  0,  0,  0,
                /* Masks and shifts */
                0,          0,          0,          0,
                0, 0,  0,  0,
        },
};

static inline const PixelFormatDescription &getDescriptionFor(const EPixelFormat fmt) {
    const int ord = static_cast<uint32_t>(fmt);
    assert(ord >= 0 && ord < static_cast<uint32_t>(EPixelFormat::Count));

    return _pixelFormats[ord];
}

String PixelUtil::GetFormatName(EPixelFormat format) {
    return getDescriptionFor(format).name;
}

uint32_t PixelUtil::GetNumElemBytes(EPixelFormat format) {
    return getDescriptionFor(format).elemBytes;
}

uint32_t PixelUtil::GetBlockSize(EPixelFormat format) {
    return GetNumElemBytes(format);
}

uint32_t PixelUtil::GetMemorySize(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format) {
    return width * height * depth * GetBlockSize(format);
}

void PixelUtil::GetSizeForMipLevel(uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevel,
                                   uint32_t &mipWidth, uint32_t &mipHeight, uint32_t &mipDepth) {
    mipWidth = width;
    mipHeight = height;
    mipDepth = depth;

    for (uint32_t i = 0; i < mipLevel; i++) {
        if (mipWidth != 1) {
            mipWidth /= 2;
        }
        if (mipHeight != 1) {
            mipHeight /= 2;
        }
        if (mipDepth != 1) {
            mipDepth /= 2;
        }
    }
}

void PixelUtil::GetPitch(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format, uint32_t &rowPitch,
                         uint32_t &depthPitch) {
    uint32_t blockSize = GetBlockSize(format);

    rowPitch = width * blockSize;
    depthPitch = width * height * blockSize;
}

EPixelFormatFlags PixelUtil::GetFlags(EPixelFormat format) {
    return getDescriptionFor(format).flags;
}

bool PixelUtil::HasAlpha(EPixelFormat format) {
    return (PixelUtil::GetFlags(format) & EPixelFormatFlags::HasAlpha) == EPixelFormatFlags::HasAlpha;
}

bool PixelUtil::IsFloatingPoint(EPixelFormat format) {
    return (PixelUtil::GetFlags(format) & EPixelFormatFlags::Float) == EPixelFormatFlags::Float;
}

bool PixelUtil::IsCompressed(EPixelFormat format) {
    return (PixelUtil::GetFlags(format) & EPixelFormatFlags::Compressed) == EPixelFormatFlags::Compressed;
}

bool PixelUtil::IsNormalized(EPixelFormat format) {
    return (PixelUtil::GetFlags(format) & EPixelFormatFlags::Normalized) == EPixelFormatFlags::Normalized;
}

bool PixelUtil::IsDepth(EPixelFormat format) {
    return (PixelUtil::GetFlags(format) & EPixelFormatFlags::Depth) == EPixelFormatFlags::Depth;
}

bool PixelUtil::CheckFormat(EPixelFormat &format, ETextureType type, ETextureUsage usage) {
    // Depth-Stencil은 Depth 포멧만 지원함
    if ((usage & ETextureUsage::DepthStencil) == ETextureUsage::DepthStencil) {
        if (IsDepth(format)) {
            return true;
        }

        //TODO: 픽셀 포멧을 d32 s8x24로 설정해야함
        format = EPixelFormat::Unknown;
        return false;
    }

    // 렌더 타겟은 압축 또는 Depth-Stencil 포멧만 제외하고 전부 지원함
    if ((usage & ETextureUsage::RenderTarget) == ETextureUsage::RenderTarget) {
        if (!IsDepth(format) && !IsCompressed(format)) {
            return true;
        }

        format = EPixelFormat::RGBA8;
        return false;
    }

    // 렌더 타겟은 압축 또는 Depth-Stencil 포멧만 제외하고 전부 지원함
    if ((usage & ETextureUsage::LoadStore) == ETextureUsage::LoadStore) {
        if (!IsDepth(format) && !IsCompressed(format)) {
            return true;
        }

        format = EPixelFormat::RGBA8;
        return false;
    }

    switch (type) {
        case ETextureType::e2D:
            if (!IsDepth(format) && !IsCompressed(format)) {
                return true;
            }

            format = EPixelFormat::RGBA8;
            return false;

        default:
            if (!IsDepth(format)) {
                return true;
            }

            format = EPixelFormat::RGBA8;
            return false;
    }
}

uint32_t PixelUtil::GetMaxMipmaps(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format) {
    auto count = 0;
    if ((width > 0) && (height > 0)) {
        while (!(width == 1 && height == 1 && depth == 1)) {
            if (width > 1) width = width / 2;
            if (height > 1) height = height / 2;
            if (depth > 1) depth = depth / 2;

            count++;
        }
    }

    return count;
}

void PixelUtil::PackColor(const Color &color, EPixelFormat format, void *dest) {
    PackColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha(), format, dest);
}

void PixelUtil::PackColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a, EPixelFormat format, void *dest) {
    const PixelFormatDescription &des = getDescriptionFor(format);

    if ((des.flags & EPixelFormatFlags::Integer) == EPixelFormatFlags::Integer) {
        // Shortcut for integer formats packing
        uint32_t value = ((Bitwise::FixedToFixed(r, 8, des.rbits) << des.rshift) & des.rmask) |
                         ((Bitwise::FixedToFixed(g, 8, des.gbits) << des.gshift) & des.gmask) |
                         ((Bitwise::FixedToFixed(b, 8, des.bbits) << des.bshift) & des.bmask) |
                         ((Bitwise::FixedToFixed(a, 8, des.abits) << des.ashift) & des.amask);

        // And write to memory
        Bitwise::IntWrite(dest, des.elemBytes, value);
    } else {
        // Convert to float
        PackColor((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f, (float) a / 255.0f, format, dest);
    }
}

void PixelUtil::PackColor(float r, float g, float b, float a, const EPixelFormat format, void *dest) {
// All other formats handled in a generic way
    const PixelFormatDescription &des = getDescriptionFor(format);
    assert(des.componentCount <= 4);

    float inputs[] = {r, g, b, a};
    uint8_t bits[] = {des.rbits, des.gbits, des.bbits, des.abits};
    uint32_t masks[] = {des.rmask, des.gmask, des.bmask, des.amask};
    uint8_t shifts[] = {des.rshift, des.gshift, des.bshift, des.ashift};

    memset(dest, 0, des.elemBytes);

    uint32_t curBit = 0;
    uint32_t prevDword = 0;
    uint32_t dwordValue = 0;
    for (uint32_t i = 0; i < des.componentCount; i++) {
        uint32_t curDword = curBit / 32;

        // New dword reached, write current one and advance
        if (curDword > prevDword) {
            uint32_t * curDst = ((uint32_t *) dest) + prevDword;
            Bitwise::IntWrite(curDst, 4, dwordValue);

            dwordValue = 0;
            prevDword = curDword;
        }

        if ((des.flags & EPixelFormatFlags::Integer) == EPixelFormatFlags::Integer) {
            if ((des.flags & EPixelFormatFlags::Normalized) == EPixelFormatFlags::Normalized) {
                if ((des.flags & EPixelFormatFlags::Signed) == EPixelFormatFlags::Signed)
                    dwordValue |= (Bitwise::SNormToUInt(inputs[i], bits[i]) << shifts[i]) & masks[i];
                else
                    dwordValue |= (Bitwise::UNormToUInt(inputs[i], bits[i]) << shifts[i]) & masks[i];
            } else {
                // Note: Casting integer to float. A better option would be to have a separate unpackColor that has
                // integer output parameters.
                dwordValue |= (((uint32_t) inputs[i]) << shifts[i]) & masks[i];
            }
        } else if ((des.flags & EPixelFormatFlags::Float) == EPixelFormatFlags::Float) {
            // Note: Not handling unsigned floats

            if (des.componentType == EPixelComponentType::Float16)
                dwordValue |= (Bitwise::FloatToHalf(inputs[i]) << shifts[i]) & masks[i];
            else
                dwordValue |= *(uint32_t *) &inputs[i];
        } else {
            LOG(LogResource, Error, TEXT("packColor() not implemented for format \"%ls\"."), *GetFormatName(format));
            return;
        }

        curBit += bits[i];
    }

    // Write last dword
    uint32_t numBytes = std::min((prevDword + 1) * 4, (uint32_t) des.elemBytes) - (prevDword * 4);
    uint32_t * curDst = ((uint32_t *) dest) + prevDword;
    Bitwise::IntWrite(curDst, numBytes, dwordValue);
}

void PixelUtil::UnpackColor(Color *color, EPixelFormat format, const void *src) {
    float r, g, b, a;
    UnpackColor(&r, &g, &b, &a, format, src);

    color->setRed(r);
    color->setGreen(g);
    color->setBlue(b);
    color->setAlpha(a);
}

void PixelUtil::UnpackColor(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a, EPixelFormat format, const void *src) {
    const PixelFormatDescription &des = getDescriptionFor(format);

    if ((des.flags & EPixelFormatFlags::Integer) == EPixelFormatFlags::Integer) {
        // Shortcut for integer formats unpacking
        const uint32_t value = Bitwise::IntRead(src, des.elemBytes);

        *r = (uint8_t) Bitwise::FixedToFixed((value & des.rmask) >> des.rshift, des.rbits, 8);
        *g = (uint8_t) Bitwise::FixedToFixed((value & des.gmask) >> des.gshift, des.gbits, 8);
        *b = (uint8_t) Bitwise::FixedToFixed((value & des.bmask) >> des.bshift, des.bbits, 8);

        if ((des.flags & EPixelFormatFlags::HasAlpha) == EPixelFormatFlags::HasAlpha) {
            *a = (uint8_t) Bitwise::FixedToFixed((value & des.amask) >> des.ashift, des.abits, 8);
        } else {
            *a = 255; // No alpha, default a component to full
        }
    } else {
        // Do the operation with the more generic floating point
        float rr, gg, bb, aa;
        UnpackColor(&rr, &gg, &bb, &aa, format, src);

        *r = (uint8_t) Bitwise::UNormToUInt(rr, 8);
        *g = (uint8_t) Bitwise::UNormToUInt(gg, 8);
        *b = (uint8_t) Bitwise::UNormToUInt(bb, 8);
        *a = (uint8_t) Bitwise::UNormToUInt(aa, 8);
    }
}

void PixelUtil::UnpackColor(float *r, float *g, float *b, float *a, EPixelFormat format, const void *src) {
    const PixelFormatDescription &des = getDescriptionFor(format);
    assert(des.componentCount <= 4);

    float *outputs[] = {r, g, b, a};
    uint8_t bits[] = {des.rbits, des.gbits, des.bbits, des.abits};
    uint32_t masks[] = {des.rmask, des.gmask, des.bmask, des.amask};
    uint8_t shifts[] = {des.rshift, des.gshift, des.bshift, des.ashift};

    uint32_t curBit = 0;
    for (uint32_t i = 0; i < des.componentCount; i++) {
        uint32_t curDword = curBit / 32;
        uint32_t numBytes = std::min((curDword + 1) * 4, (uint32_t) des.elemBytes) - (curDword * 4);

        uint32_t * curSrc = ((uint32_t *) src) + curDword;
        uint32_t value = Bitwise::IntRead(curSrc, numBytes);
        if ((des.flags & EPixelFormatFlags::Integer) == EPixelFormatFlags::Integer) {
            if ((des.flags & EPixelFormatFlags::Normalized) == EPixelFormatFlags::Normalized) {
                if ((des.flags & EPixelFormatFlags::Signed) == EPixelFormatFlags::Signed)
                    *outputs[i] = Bitwise::UIntToSNorm((value & masks[i]) >> shifts[i], bits[i]);
                else
                    *outputs[i] = Bitwise::UIntToUNorm((value & masks[i]) >> shifts[i], bits[i]);
            } else {
                // Note: Casting integer to float. A better option would be to have a separate unpackColor that has
                // integer output parameters.
                *outputs[i] = (float) ((value & masks[i]) >> shifts[i]);
            }
        } else if ((des.flags & EPixelFormatFlags::Float) == EPixelFormatFlags::Float) {
            // Note: Not handling unsigned floats

            if (des.componentType == EPixelComponentType::Float16)
                *outputs[i] = Bitwise::HalfToFloat((uint16_t) ((value & masks[i]) >> shifts[i]));
            else
                *outputs[i] = *(float *) &value;
        } else {
            LOG(LogResource, Error, TEXT("unpackColor() not implemented for format \"%ls\"."), *GetFormatName(format));
            return;
        }

        curBit += bits[i];
    }

    // Fill empty components
    for (uint32_t i = des.componentCount; i < 3; i++)
        *outputs[i] = 0.0f;

    if (des.componentCount < 4)
        *outputs[3] = 1.0f;
}
