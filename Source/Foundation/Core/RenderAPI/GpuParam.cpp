#include "GpuParam.h"

#include "Exception/Exception.h"
#include "RenderAPI/GpuParams.h"
#include "RenderAPI/RenderAPI.h"
#include "Image/Color.h"
#include "GpuParamBlockBuffer.h"

template<class T>
FGpuDataParam<T>::FGpuDataParam() : mParamDesc(nullptr) {}

template<class T>
FGpuDataParam<T>::FGpuDataParam(FGpuParamDataDesc *paramDesc, GpuParamsType *parent)
        : mParamDesc(paramDesc), mParent(parent) {}

template<class T>
void FGpuDataParam<T>::set(const T &value, uint32_t arrayIdx) const {
    if (mParent == nullptr) {
        return;
    }

    auto paramBlock = mParent->getParamBlockBuffer(mParamDesc->paramBlockSet, mParamDesc->paramBlockSlot);
    if (paramBlock == nullptr) {
        return;
    }

#if DEBUG_MODE
    if (arrayIdx >= mParamDesc->arraySize) {
        EXCEPT(FLogRenderAPI, InvalidParametersException, TEXT("Array gIBO out of range. Array size: %ld. Requested size: %ld"), mParamDesc->arraySize, arrayIdx);
    }
#endif

    uint32_t elementSizeBytes = mParamDesc->elementSize * sizeof(uint32_t);
    uint32_t sizeBytes = std::min(elementSizeBytes, static_cast<uint32_t>(sizeof(T)));

    const bool transposeMatrices = false;// gCaps().conventions.matrixOrder == FConventions::EMatrixOrder::ColumnMajor;
    if (FTransposePolicy<T>::TransposeEnabled(transposeMatrices)) {
        auto transposed = FTransposePolicy<T>::Transpose(value);
        paramBlock->write((mParamDesc->cpuMemOffset + arrayIdx * mParamDesc->arrayElementStride) * sizeof(uint32_t), &transposed, sizeBytes);
    } else {
        paramBlock->write((mParamDesc->cpuMemOffset + arrayIdx * mParamDesc->arrayElementStride) * sizeof(uint32_t), &value, sizeBytes);
    }

    if (sizeBytes < elementSizeBytes) {
        uint32_t diffSize = elementSizeBytes - sizeBytes;
        paramBlock->zeroOut((mParamDesc->cpuMemOffset + arrayIdx * mParamDesc->arrayElementStride) *
                            sizeof(uint32_t) + sizeBytes, diffSize);
    }
}

template<class T>
T FGpuDataParam<T>::get(uint32_t arrayIdx) {
    if (mParent == nullptr) {
        return T();
    }

    auto paramBlock = mParent->getParamBlockBuffer(mParamDesc->paramBlockSet, mParamDesc->paramBlockSlot);
    if (paramBlock == nullptr)
        return T();

#if DEBUG_MODE
    if (arrayIdx >= mParamDesc->arraySize) {
        EXCEPT(FLogRenderAPI, InvalidParametersException, TEXT("Array gIBO out of range. Array size: %ld. Requested size: %ld"), mParamDesc->arraySize, arrayIdx);
    }
#endif

    uint32_t elementSizeBytes = mParamDesc->elementSize * sizeof(uint32_t);
    uint32_t sizeBytes = std::min(elementSizeBytes, static_cast<uint32_t>(sizeof(T)));

    T value;
    paramBlock->read((mParamDesc->cpuMemOffset + arrayIdx * mParamDesc->arrayElementStride) * sizeof(uint32_t),
                     &value, sizeBytes);

    return value;
}

template class FGpuDataParam<float>;
template class FGpuDataParam<int>;
template class FGpuDataParam<FColor>;
template class FGpuDataParam<FVector2>;
template class FGpuDataParam<FVector3>;
template class FGpuDataParam<FMatrix4>;

FGpuParamBuffer::FGpuParamBuffer() : mParamDesc(nullptr) { }

FGpuParamBuffer::FGpuParamBuffer(FGpuParamObjectDesc *paramDesc, FGpuParamBuffer::GpuParamsType *parent)
        : mParamDesc(paramDesc), mParent(parent) { }

void FGpuParamBuffer::set(FGpuParamBuffer::BufferType *buffer) const {
    if (mParent == nullptr) {
        return;
    }

    mParent->setBuffer(mParamDesc->set, mParamDesc->slot, buffer);
}

FGpuParamBuffer::BufferType *FGpuParamBuffer::get() const {
    if (mParent == nullptr) {
        return nullptr;
    }

    return mParent->getBuffer(mParamDesc->set, mParamDesc->slot);
}

FGpuParamTexture::FGpuParamTexture() : mParamDesc(nullptr) { }

FGpuParamTexture::FGpuParamTexture(FGpuParamObjectDesc *paramDesc, FGpuParamTexture::GpuParamsType *parent)
    : mParamDesc(paramDesc), mParent(parent) { }

void FGpuParamTexture::set(const FGpuParamTexture::TextureType &texture, const FTextureSurface &surface) const {
    if (mParent == nullptr) {
        return;
    }

    mParent->setTexture(mParamDesc->set, mParamDesc->slot, texture, surface);
}

FGpuParamTexture::TextureType FGpuParamTexture::get() const {
    if (mParent == nullptr) {
        return TextureType();
    }

    return mParent->getTexture(mParamDesc->set, mParamDesc->slot);
}

FGpuParamSamplerState::FGpuParamSamplerState() : mParamDesc(nullptr) {

}

FGpuParamSamplerState::FGpuParamSamplerState(FGpuParamObjectDesc *paramDesc, FGpuParamSamplerState::GpuParamsType *parent)
        : mParamDesc(paramDesc), mParent(parent) { }

void FGpuParamSamplerState::set(FGpuParamSamplerState::SamplerType *samplerState) const {
    if (mParent == nullptr) {
        return;
    }

    mParent->setSamplerState(mParamDesc->set, mParamDesc->slot, samplerState);
}

FGpuParamSamplerState::SamplerType *FGpuParamSamplerState::get() const {
    if (mParent == nullptr) {
        return nullptr;
    }

    return mParent->getSamplerState(mParamDesc->set, mParamDesc->slot);
}
