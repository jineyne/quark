#include "PixelData.h"
#include "Exception/Exception.h"

FPixelData::FPixelData(const FPixelVolume &extents, EPixelFormat pixelFormat) : mExtents(extents), mFormat(pixelFormat) {
    FPixelUtil::GetPitch(extents.getWidth(), extents.getHeight(), extents.getDepth(), pixelFormat, mRowPitch, mSlicePitch);
}

FPixelData::FPixelData(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat pixelFormat) : mExtents(0, 0, 0, width, height, depth), mFormat(pixelFormat) {
    FPixelUtil::GetPitch(width, height, depth, pixelFormat, mRowPitch, mSlicePitch);
}

FPixelData::FPixelData(const FPixelData &copy) : FGpuResourceData(copy)  {
    mFormat = copy.mFormat;
    mRowPitch = copy.mRowPitch;
    mSlicePitch = copy.mSlicePitch;
    mExtents = copy.mExtents;
}

FPixelData *FPixelData::New(uint32_t width, uint32_t height, uint32_t depth, EPixelFormat format) {
    auto data = q_new<FPixelData>(width, height, depth, format);
    data->allocateInternalBuffer();
    return data;
}

FPixelData &FPixelData::operator=(const FPixelData &rhs) {
    FGpuResourceData::operator=(rhs);

    mFormat = rhs.mFormat;
    mRowPitch = rhs.mRowPitch;
    mSlicePitch = rhs.mSlicePitch;
    mExtents = rhs.mExtents;

    return *this;
}

uint32_t FPixelData::getConsecutiveSize() const {
    return FPixelUtil::GetMemorySize(getWidth(), getHeight(), getDepth(), mFormat);
}

FPixelData *FPixelData::getSubVolume(const FPixelVolume &volume) const {
    if (!mExtents.contains(volume)) {
        EXCEPT(FLogResource, InvalidParametersException, TEXT("Bounds out of range"));
    }

    const size_t elemSize = FPixelUtil::GetNumElemBytes(mFormat);
    FPixelData *rval = new FPixelData(volume.getWidth(), volume.getHeight(), volume.getDepth(), mFormat);

    rval->setExternalBuffer(((uint8_t *) getData()) + ((volume.left - getLeft()) * elemSize) +
                            ((volume.top - getTop()) * mRowPitch) +
                            ((volume.front - getFront()) * mSlicePitch));

    rval->mFormat = mFormat;
    FPixelUtil::GetPitch(volume.getWidth(), volume.getHeight(), volume.getDepth(), mFormat, rval->mRowPitch,
                         rval->mSlicePitch);

    return rval;
}

FColor FPixelData::getColorAt(uint32_t x, uint32_t y, uint32_t z) const {
    FColor cv;

    uint32_t pixelSize = FPixelUtil::GetNumElemBytes(mFormat);
    uint32_t pixelOffset = z * mSlicePitch + y * mRowPitch + x * pixelSize;
    FPixelUtil::UnpackColor(&cv, mFormat, (unsigned char *) getData() + pixelOffset);

    return cv;
}

void FPixelData::setColorAt(const FColor &color, uint32_t x, uint32_t y, uint32_t z) {
    uint32_t pixelSize = FPixelUtil::GetNumElemBytes(mFormat);
    uint32_t pixelOffset = z * mSlicePitch + y * mRowPitch + x * pixelSize;
    FPixelUtil::PackColor(color, mFormat, (unsigned char *) getData() + pixelOffset);
}

TArray<FColor> FPixelData::getColors() const {
    uint32_t depth = mExtents.getDepth();
    uint32_t height = mExtents.getHeight();
    uint32_t width = mExtents.getWidth();

    uint32_t pixelSize = FPixelUtil::GetNumElemBytes(mFormat);
    uint8_t *data = getData();

    TArray<FColor> colors(width * height * depth);
    for (uint32_t z = 0; z < depth; z++) {
        uint32_t zArrayIdx = z * width * height;
        uint32_t zDataIdx = z * mSlicePitch;

        for (uint32_t y = 0; y < height; y++) {
            uint32_t yArrayIdx = y * width;
            uint32_t yDataIdx = y * mRowPitch;

            for (uint32_t x = 0; x < width; x++) {
                uint32_t arrayIdx = x + yArrayIdx + zArrayIdx;
                uint32_t dataIdx = x * pixelSize + yDataIdx + zDataIdx;

                uint8_t *dest = data + dataIdx;
                FPixelUtil::UnpackColor(&colors[arrayIdx], mFormat, dest);
            }
        }
    }

    return colors;
}

void FPixelData::setColors(const TArray<FColor> &colors) {
    setColorsInternal(colors, static_cast<uint32_t>(colors.length()));
}

void FPixelData::setColors(FColor *colors, uint32_t elementCount) {
    setColorsInternal(colors, elementCount);
}

void FPixelData::setColors(const FColor &color) {
    uint32_t depth = mExtents.getDepth();
    uint32_t height = mExtents.getHeight();
    uint32_t width = mExtents.getWidth();

    uint32_t pixelSize = FPixelUtil::GetNumElemBytes(mFormat);
    uint32_t packedColor[4];
    assert(pixelSize <= sizeof(packedColor));

    FPixelUtil::PackColor(color, mFormat, packedColor);

    uint8_t *data = getData();
    for (uint32_t z = 0; z < depth; z++) {
        uint32_t zDataIdx = z * mSlicePitch;

        for (uint32_t y = 0; y < height; y++) {
            uint32_t yDataIdx = y * mRowPitch;

            for (uint32_t x = 0; x < width; x++) {
                uint32_t dataIdx = x * pixelSize + yDataIdx + zDataIdx;

                uint8_t *dest = data + dataIdx;
                memcpy(dest, packedColor, pixelSize);
            }
        }
    }
}

template<class T>
void FPixelData::setColorsInternal(const T &colors, uint32_t numElements) {
    uint32_t depth = mExtents.getDepth();
    uint32_t height = mExtents.getHeight();
    uint32_t width = mExtents.getWidth();

    uint32_t totalNumElements = width * height * depth;
    if (numElements != totalNumElements) {
        EXCEPT(FLogResource, InvalidParametersException, TEXT("Unable to set colors, invalid array size."));
        return;
    }

    uint32_t pixelSize = FPixelUtil::GetNumElemBytes(mFormat);
    uint8_t *data = getData();

    for (uint32_t z = 0; z < depth; z++) {
        uint32_t zArrayIdx = z * width * height;
        uint32_t zDataIdx = z * mSlicePitch;

        for (uint32_t y = 0; y < height; y++) {
            uint32_t yArrayIdx = y * width;
            uint32_t yDataIdx = y * mRowPitch;

            for (uint32_t x = 0; x < width; x++) {
                uint32_t arrayIdx = x + yArrayIdx + zArrayIdx;
                uint32_t dataIdx = x * pixelSize + yDataIdx + zDataIdx;

                uint8_t *dest = data + dataIdx;
                FPixelUtil::PackColor(colors[arrayIdx], mFormat, dest);
            }
        }
    }
}

template DLL_EXPORT void FPixelData::setColorsInternal(FColor *const &, uint32_t);
template DLL_EXPORT void FPixelData::setColorsInternal(const TArray<FColor> &, uint32_t);

uint32_t FPixelData::getInternalBufferSize() const {
    return getSize();
}
