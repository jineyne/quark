#pragma once

#include "CorePrerequisites.h"
#include "Resource/GpuResourceData.h"
#include "Image/Color.h"
#include "PixelVolume.h"
#include "PixelUtil.h"

class DLL_EXPORT FPixelData  : public FGpuResourceData  {
private:
    FPixelVolume mExtents = FPixelVolume(0, 0, 0, 0);
    EPixelFormat mFormat = EPixelFormat::Unknown;

    uint32_t mRowPitch = 0;
    uint32_t mSlicePitch = 0;

public:
    FPixelData() = default;
    FPixelData(const FPixelVolume& extents, EPixelFormat pixelFormat);
    FPixelData(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat pixelFormat);
    FPixelData(const FPixelData &copy);

public:
    static FPixelData *New(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format);

public:
    FPixelData &operator=(const FPixelData &rhs);

public:
    uint32_t getRowPitch() const { return mRowPitch; }
    void setRowPitch(uint32_t rowPitch) { mRowPitch = rowPitch; }

    uint32_t getSlicePitch() const { return mSlicePitch; }
    void setSlicePitch(uint32_t slicePitch) { mSlicePitch = slicePitch; }

    EPixelFormat getFormat() const { return mFormat; }
    const FPixelVolume &getExtents() const { return mExtents; }
    uint32_t getWidth() const { return mExtents.getWidth(); }
    uint32_t getHeight() const { return mExtents.getHeight(); }
    uint32_t getDepth() const { return mExtents.getDepth(); }
    uint32_t getLeft() const { return mExtents.left; }
    uint32_t getRight() const { return mExtents.right; }
    uint32_t getTop() const { return mExtents.top; }
    uint32_t getBottom() const { return mExtents.bottom; }
    uint32_t getFront() const { return mExtents.front; }
    uint32_t getBack() const { return mExtents.back; }
    uint32_t getSize() const { return mSlicePitch * getDepth(); }
    uint32_t getConsecutiveSize() const;

    FPixelData *getSubVolume(const FPixelVolume &volume) const;

    FColor getColorAt(uint32_t x, uint32_t y, uint32_t z = 0) const;

    void setColorAt(const FColor &color, uint32_t x, uint32_t y, uint32_t z = 0);

    TArray<FColor> getColors() const;

    void setColors(const TArray<FColor> &colors);
    void setColors(FColor *colors, uint32_t elementCount);
    void setColors(const FColor& color);

private:
    template<class T>
    void setColorsInternal(const T& colors, uint32_t numElements);
    uint32_t getInternalBufferSize() const override;

};