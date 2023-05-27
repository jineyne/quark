#pragma once

#include "CorePrerequisites.h"

enum class EGpuViewUsage {
    Default = 0x01,
    /** Buffer is seen as a render target that color pixels will be written to after pixel shader stage. */
    RenderTarget = 0x02,
    /** Buffer is seen as a depth stencil target that depth and stencil information is written to. */
    DepthStencil = 0x04,
    /** Buffer that allows you to write to any part of it from within a GPU program. */
    RandomWrite = 0x08
};

ENUM_CLASS_FLAGS(EGpuViewUsage);

struct TextureViewDesc {
    uint32_t mostDetailMip;
    uint32_t mipCount;
    uint32_t firstArraySlice;
    uint32_t arraySlicesCount;
    EGpuViewUsage usage;
};

class DLL_EXPORT TextureView {
public:
    class HashFunction {
    public:
        size_t operator()(const TextureViewDesc &key) const;
    };

    class EqualFunction {
    public:
        bool operator()(const TextureViewDesc &a, const TextureViewDesc &b) const;
    };

protected:
    TextureViewDesc mDesc;

protected:
    TextureView(const TextureViewDesc &desc);

public:
    virtual ~TextureView() = default;

public:
    /**	Returns the most detailed mip level visible by the view. */
    uint32_t getMostDetailedMip() const { return mDesc.mostDetailMip; }

    /**	Returns the number of mip levels in a single slice visible by the view. */
    uint32_t getMipCount() const { return mDesc.mipCount; }

    /**	Returns the first array slice index visible by this view. */
    uint32_t getFirstArraySlice() const { return mDesc.firstArraySlice; }

    /**	Returns the number of array slices visible by this view. */
    uint32_t getNumArraySlices() const { return mDesc.arraySlicesCount; }

    /**	Returns texture view usage. This determines where on the pipeline can be bind the view. */
    EGpuViewUsage getUsage() const { return mDesc.usage; }

    /**	Returns the descriptor structure used for initializing the view. */
    const TextureViewDesc& getDesc() const { return mDesc; }

private:
    friend class Texture;
};
