#include "GpuParam.h"

#include "Exception/Exception.h"
#include "RenderAPI/GpuParams.h"
#include "RenderAPI/RenderAPI.h"
#include "Image/Color.h"
#include "GpuParamBlockBuffer.h"

template<class T>
GpuDataParam<T>::GpuDataParam() : mParamDesc(nullptr) {}

template<class T>
GpuDataParam<T>::GpuDataParam(GpuParamDataDesc *paramDesc, GpuParamsType *parent)
        : mParamDesc(paramDesc), mParent(parent) {}

template<class T>
void GpuDataParam<T>::set(const T &value, uint32_t arrayIdx) const {
    if (mParent == nullptr) {
        return;
    }

    auto paramBlock = mParent->getParamBlockBuffer(mParamDesc->paramBlockSet, mParamDesc->paramBlockSlot);
    if (paramBlock == nullptr) {
        return;
    }

#if DEBUG_MODE
    if (arrayIdx >= mParamDesc->arraySize) {
        EXCEPT(LogRenderAPI, InvalidParametersException, TEXT("Array gIBO out of range. Array size: %ld. Requested size: %ld"), mParamDesc->arraySize, arrayIdx);
    }
#endif

    uint32_t elementSizeBytes = mParamDesc->elementSize * sizeof(uint32_t);
    uint32_t sizeBytes = std::min(elementSizeBytes, static_cast<uint32_t>(sizeof(T)));

    const bool transposeMatrices = false;// gCaps().conventions.matrixOrder == Conventions::EMatrixOrder::ColumnMajor;
    if (TTransposePolicy<T>::TransposeEnabled(transposeMatrices)) {
        auto transposed = TTransposePolicy<T>::Transpose(value);
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
T GpuDataParam<T>::get(uint32_t arrayIdx) {
    if (mParent == nullptr) {
        return T();
    }

    auto paramBlock = mParent->getParamBlockBuffer(mParamDesc->paramBlockSet, mParamDesc->paramBlockSlot);
    if (paramBlock == nullptr)
        return T();

#if DEBUG_MODE
    if (arrayIdx >= mParamDesc->arraySize) {
        EXCEPT(LogRenderAPI, InvalidParametersException, TEXT("Array gIBO out of range. Array size: %ld. Requested size: %ld"), mParamDesc->arraySize, arrayIdx);
    }
#endif

    uint32_t elementSizeBytes = mParamDesc->elementSize * sizeof(uint32_t);
    uint32_t sizeBytes = std::min(elementSizeBytes, static_cast<uint32_t>(sizeof(T)));

    T value;
    paramBlock->read((mParamDesc->cpuMemOffset + arrayIdx * mParamDesc->arrayElementStride) * sizeof(uint32_t),
                     &value, sizeBytes);

    return value;
}

template class GpuDataParam<float>;
template class GpuDataParam<int>;
template class GpuDataParam<Color>;
template class GpuDataParam<Vector2>;
template class GpuDataParam<Vector3>;
template class GpuDataParam<Matrix4>;

GpuParamBuffer::GpuParamBuffer() : mParamDesc(nullptr) { }

GpuParamBuffer::GpuParamBuffer(GpuParamObjectDesc *paramDesc, GpuParamBuffer::GpuParamsType *parent)
        : mParamDesc(paramDesc), mParent(parent) { }

void GpuParamBuffer::set(GpuParamBuffer::BufferType *buffer) const {
    if (mParent == nullptr) {
        return;
    }

    mParent->setBuffer(mParamDesc->set, mParamDesc->slot, buffer);
}

GpuParamBuffer::BufferType *GpuParamBuffer::get() const {
    if (mParent == nullptr) {
        return nullptr;
    }

    return mParent->getBuffer(mParamDesc->set, mParamDesc->slot);
}

GpuParamTexture::GpuParamTexture() : mParamDesc(nullptr) { }

GpuParamTexture::GpuParamTexture(GpuParamObjectDesc *paramDesc, GpuParamTexture::GpuParamsType *parent)
    : mParamDesc(paramDesc), mParent(parent) { }

void GpuParamTexture::set(const GpuParamTexture::TextureType &texture, const TextureSurface &surface) const {
    if (mParent == nullptr) {
        return;
    }

    mParent->setTexture(mParamDesc->set, mParamDesc->slot, texture, surface);
}

GpuParamTexture::TextureType GpuParamTexture::get() const {
    if (mParent == nullptr) {
        return TextureType();
    }

    return mParent->getTexture(mParamDesc->set, mParamDesc->slot);
}

GpuParamSamplerState::GpuParamSamplerState() : mParamDesc(nullptr) {

}

GpuParamSamplerState::GpuParamSamplerState(GpuParamObjectDesc *paramDesc, GpuParamSamplerState::GpuParamsType *parent)
        : mParamDesc(paramDesc), mParent(parent) { }

void GpuParamSamplerState::set(GpuParamSamplerState::SamplerType *samplerState) const {
    if (mParent == nullptr) {
        return;
    }

    mParent->setSamplerState(mParamDesc->set, mParamDesc->slot, samplerState);
}

GpuParamSamplerState::SamplerType *GpuParamSamplerState::get() const {
    if (mParent == nullptr) {
        return nullptr;
    }

    return mParent->getSamplerState(mParamDesc->set, mParamDesc->slot);
}
