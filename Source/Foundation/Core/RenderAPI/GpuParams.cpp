#include "GpuParams.h"
#include "Manager/BufferManager.h"
#include "GpuParamBlockBuffer.h"
#include "GraphicsPipelineState.h"
#include "RenderAPI.h"

const GpuDataParamInfos GpuParams::ParamSizes;

FGpuParamsBase::FGpuParamsBase(FGpuPipelineParamInfoBase *paramInfo) : mParamInfo(paramInfo) {

}

FGpuParamsBase::~FGpuParamsBase() {
    q_delete(mParamInfo);
}

GpuParams *FGpuParamsBase::New(FGpuPipelineParamInfo *paramInfo) {
    return BufferManager::Instance().createGpuParams(paramInfo);
}

GpuParams *FGpuParamsBase::New(GraphicsPipelineState *pipelineState) {
    return New(pipelineState->getParamInfo());
}

GpuParamDesc *FGpuParamsBase::getParamDesc(EGpuProgramType type) const {
    return mParamInfo->getParamDesc(type);
}

uint32_t FGpuParamsBase::getDataParamSize(EGpuProgramType type, const String &name) const {
    auto desc = getParamDesc(type, name);
    if (desc != nullptr) {
        return desc->elementSize * 4;
    }

    return 0;
}

bool FGpuParamsBase::hasParam(EGpuProgramType type, const String &name) const {
    return getParamDesc(type, name) != nullptr;
}

bool FGpuParamsBase::hasTexture(EGpuProgramType type, const String &name) const {
    auto paramDesc = mParamInfo->getParamDesc(type);
    if (paramDesc == nullptr) {
        return false;
    }

    auto it = paramDesc->textures.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool FGpuParamsBase::hasLoadStoreTexture(EGpuProgramType type, const String &name) const {
    auto paramDesc = mParamInfo->getParamDesc(type);
    if (paramDesc == nullptr) {
        return false;
    }

    auto it = paramDesc->loadStoreTextures.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool FGpuParamsBase::hasBuffer(EGpuProgramType type, const String &name) const {
    auto paramDesc = mParamInfo->getParamDesc(type);
    if (paramDesc == nullptr) {
        return false;
    }

    auto it = paramDesc->buffers.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool FGpuParamsBase::hasSamplerState(EGpuProgramType type, const String &name) const {
    auto paramDesc = mParamInfo->getParamDesc(type);
    if (paramDesc == nullptr) {
        return false;
    }

    auto it = paramDesc->samplers.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

bool FGpuParamsBase::hasParamBlock(EGpuProgramType type, const String &name) const {
    auto paramDesc = mParamInfo->getParamDesc(type);
    if (paramDesc == nullptr) {
        return false;
    }

    auto it = paramDesc->paramBlocks.find(name);
    if (it != nullptr) {
        return true;
    }

    return false;
}

GpuParamBlockDesc *FGpuParamsBase::getParamBlockDesc(EGpuProgramType type, const String &name) const {
    auto paramDesc = mParamInfo->getParamDesc(type);
    if (paramDesc == nullptr) {
        return nullptr;
    }

    auto it = paramDesc->paramBlocks.find(name);
    if (it != nullptr) {
        return &*it;
    }

    return nullptr;
}

GpuParamDataDesc *FGpuParamsBase::getParamDesc(EGpuProgramType type, const String &name) const {
    auto paramDesc = mParamInfo->getParamDesc(type);
    if (paramDesc == nullptr) {
        return nullptr;
    }

    auto it = paramDesc->params.find(name);
    if (it != nullptr) {
        return &*it;
    }

    return nullptr;
}

GpuParams::GpuParams(FGpuPipelineParamInfoBase *paramInfo) : FGpuParamsBase(paramInfo) {
    uint32_t paramBlockCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::ParamBlock);
    uint32_t buffersCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::Buffer);
    uint32_t textureCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::Texture);
    uint32_t samplerCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::SamplerState);

    auto paramBlocksSize = sizeof(ParamsBufferType) * paramBlockCount;
    auto buffersSize = sizeof(BufferType) * buffersCount;
    auto texturesSize = (sizeof(TextureType) + sizeof(TextureSurface)) * textureCount;
    auto samplersSize = sizeof(BufferType) * samplerCount;

    auto totalSize = paramBlocksSize + buffersSize + texturesSize + samplersSize;

    auto *data = (uint8_t *) malloc(totalSize);

    mParamBlockBuffers = (ParamsBufferType *) data;
    for (uint32_t i = 0; i < paramBlockCount; i++) {
        new (&mParamBlockBuffers[i]) ParamsBufferType();
    }

    data += paramBlocksSize;
    mBuffers = (BufferType *) data;
    for (uint32_t i = 0; i < buffersCount; i++) {
        new (&mBuffers[i]) BufferType();
    }

    data += buffersSize;
    mSampledTextureData = (TextureData *) data;
    for (uint32_t i = 0; i < textureCount; i++) {
        new (&mSampledTextureData[i].texture) TextureType();
        new (&mSampledTextureData[i].surface) TextureSurface(0, 0, 0, 0);
    }

    data += texturesSize;
    mSamplerStates = (SamplerType *) data;
    for (uint32_t i = 0; i < samplerCount; i++) {
        new (&mSamplerStates[i]) SamplerType();
    }
}

GpuParams::~GpuParams() {
    uint32_t paramBlockCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::ParamBlock);
    uint32_t buffersCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::Buffer);
    uint32_t textureCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::Texture);
    uint32_t samplerCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::SamplerState);

    for (uint32_t i = 0; i < buffersCount; i++) {
        mBuffers[i].~BufferType();
    }

    for (uint32_t i = 0; i < paramBlockCount; i++) {
        mParamBlockBuffers[i].~ParamsBufferType();
    }

    q_free(mParamBlockBuffers);
}

template<class T>
void GpuParams::getParam(EGpuProgramType type, const String &name, GpuDataParam<T> &output) const {
    const auto paramDescs = mParamInfo->getParamDesc(type);

    if (paramDescs == nullptr) {
        output = GpuDataParam<T>(nullptr, nullptr);
        LOG(LogRenderAPI, Warning, TEXT("Cannot find parameter with the name: '%ls'"), *name);
        return;
    }

    auto it = paramDescs->params.find(name);
    if (it == nullptr) {
        output = GpuDataParam<T>(nullptr, nullptr);
        LOG(LogRenderAPI, Warning, TEXT("Cannot find parameter with the name: '%ls'"), *name);
        return;
    } else {
        output = GpuDataParam<T>(&*it, const_cast<GpuParams*>(this));
    }
}

void GpuParams::getBufferParam(EGpuProgramType type, const String &name, GpuParamBuffer &output) const {
    auto desc = mParamInfo->getParamDesc(type);
    if (desc == nullptr) {
        return;
    }

    auto it = desc->buffers.find(name);
    if (it == nullptr) {
        output = GpuParamBuffer(nullptr, nullptr);
        LOG(LogRenderAPI, Warning, TEXT("Cannot find buffer parameter with the name: '%ls'"), *name);
    } else {
        output = GpuParamBuffer(&*it, const_cast<GpuParams*>(this));
    }
}

void GpuParams::getTextureParam(EGpuProgramType type, const String &name, GpuParamTexture &output) const {
    auto desc = mParamInfo->getParamDesc(type);
    if (desc == nullptr) {
        return;
    }

    auto it = desc->textures.find(name);
    if (it == nullptr) {
        output = GpuParamTexture(nullptr, nullptr);
        LOG(LogRenderAPI, Warning, TEXT("Cannot find texture parameter with the name: '%ls'"), *name);
    } else {
        output = GpuParamTexture(&*it, const_cast<GpuParams*>(this));
    }
}

void GpuParams::getSamplerStateParam(EGpuProgramType type, const String &name, GpuParamSamplerState &output) const {
    auto desc = mParamInfo->getParamDesc(type);
    if (desc == nullptr) {
        return;
    }

    auto it = desc->samplers.find(name);
    if (it == nullptr) {
        output = GpuParamSamplerState(nullptr, nullptr);
        LOG(LogRenderAPI, Warning, TEXT("Cannot find sampler state parameter with the name: '%ls'"), *name);
    } else {
        output = GpuParamSamplerState(&*it, const_cast<GpuParams*>(this));
    }
}

GpuParams::ParamsBufferType GpuParams::getParamBlockBuffer(uint32_t set, uint32_t slot) const {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::ParamBlock, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return nullptr;
    }

    return mParamBlockBuffers[globalSlot];
}

GpuParams::BufferType GpuParams::getBuffer(uint32_t set, uint32_t slot) const {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::Buffer, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return nullptr;
    }

    return mBuffers[globalSlot];
}

GpuParams::TextureType GpuParams::getTexture(uint32_t set, uint32_t slot) const {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::Texture, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return GpuParams::TextureType();
    }

    return mSampledTextureData[globalSlot].texture;
}

GpuParams::SamplerType GpuParams::getSamplerState(uint32_t set, uint32_t slot) const {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::SamplerState, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return GpuParams::SamplerType();
    }

    return mSamplerStates[globalSlot];
}

const TextureSurface &GpuParams::getTextureSurface(uint32_t set, uint32_t slot) const {
    static TextureSurface emptySurface;

    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::Texture, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return emptySurface;
    }

    return mSampledTextureData[globalSlot].surface;
}

void GpuParams::setParamBlockBuffer(EGpuProgramType type, const String &name,
                                    const GpuParams::ParamsBufferType &paramBlockBuffer) {
    const auto &paramDescs = mParamInfo->getParamDesc(type);
    if (paramDescs == nullptr) {
        LOG(LogRenderAPI, Warning, TEXT("Cannot find parameter block with the name: '%ls'"), *name);
        return;
    }

    auto it = paramDescs->paramBlocks.find(name);
    if (it == nullptr) {
        LOG(LogRenderAPI, Warning, TEXT("Cannot find parameter block with the name: '%ls'"), *name);
        return;
    }

    setParamBlockBuffer((*it).set, (*it).slot, paramBlockBuffer);
}

void GpuParams::setParamBlockBuffer(const String &name, const GpuParams::ParamsBufferType &paramBlockBuffer) {
    for (uint32_t i = 0; i < static_cast<uint32_t>(EGpuProgramType::Count); i++) {
        const auto &paramDescs = mParamInfo->getParamDesc(static_cast<EGpuProgramType>(i));
        if (paramDescs == nullptr) {
            continue;
        }

        auto it = paramDescs->paramBlocks.find(name);
        if (it == nullptr) {
            continue;
        }

        setParamBlockBuffer((*it).set, (*it).slot, paramBlockBuffer);
    }
}

void GpuParams::setParamBlockBuffer(uint32_t set, uint32_t slot, const GpuParams::ParamsBufferType &paramBlockBuffer) {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::ParamBlock, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return;
    }

    mParamBlockBuffers[globalSlot] = paramBlockBuffer;
}

void GpuParams::setBuffer(uint32_t set, uint32_t slot, const GpuParams::BufferType &buffer) {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::Buffer, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return;
    }

    mBuffers[globalSlot] = buffer;
}

void GpuParams::setTexture(uint32_t set, uint32_t slot, GpuParams::TextureType texture, const TextureSurface &surface) {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::Texture, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return;
    }

    mSampledTextureData[globalSlot].texture = texture;
    mSampledTextureData[globalSlot].surface = surface;
}

void GpuParams::setSamplerState(uint32_t set, uint32_t slot, GpuParams::SamplerType sampler) {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::SamplerState, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return;
    }

    mSamplerStates[globalSlot] = sampler;
}

template DLL_EXPORT void GpuParams::getParam<float>(EGpuProgramType type, const String &, GpuDataParam<float> &) const;
template DLL_EXPORT void GpuParams::getParam<int>(EGpuProgramType type, const String &, GpuDataParam<int> &) const;
template DLL_EXPORT void GpuParams::getParam<Vector2>(EGpuProgramType type, const String &, GpuDataParam<Vector2> &) const;
template DLL_EXPORT void GpuParams::getParam<Vector3>(EGpuProgramType type, const String &, GpuDataParam<Vector3> &) const;
template DLL_EXPORT void GpuParams::getParam<Matrix4>(EGpuProgramType type, const String &, GpuDataParam<Matrix4> &) const;
template DLL_EXPORT void GpuParams::getParam<Color>(EGpuProgramType type, const String &, GpuDataParam<Color> &) const;