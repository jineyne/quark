#include "GpuParams.h"
#include "Manager/BufferManager.h"
#include "GpuParamBlockBuffer.h"
#include "GraphicsPipelineState.h"
#include "RenderAPI.h"

FGpuParamsBase::FGpuParamsBase(FGpuPipelineParamInfoBase *paramInfo) : mParamInfo(paramInfo) {

}

FGpuParamsBase::~FGpuParamsBase() {
    delete mParamInfo;
}

FGpuParams *FGpuParamsBase::New(FGpuPipelineParamInfo *paramInfo) {
    return FBufferManager::Instance().createGpuParams(paramInfo);
}

FGpuParams *FGpuParamsBase::New(FGraphicsPipelineState *pipelineState) {
    return New(pipelineState->getParamInfo());
}

FGpuParamDesc *FGpuParamsBase::getParamDesc(EGpuProgramType type) const {
    return mParamInfo->getParamDesc(type);
}

uint32_t FGpuParamsBase::getDataParamSize(EGpuProgramType type, const FString &name) const {
    auto desc = getParamDesc(type, name);
    if (desc != nullptr) {
        return desc->elementSize * 4;
    }

    return 0;
}

bool FGpuParamsBase::hasParam(EGpuProgramType type, const FString &name) const {
    return getParamDesc(type, name) != nullptr;
}

bool FGpuParamsBase::hasTexture(EGpuProgramType type, const FString &name) const {
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

bool FGpuParamsBase::hasLoadStoreTexture(EGpuProgramType type, const FString &name) const {
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

bool FGpuParamsBase::hasBuffer(EGpuProgramType type, const FString &name) const {
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

bool FGpuParamsBase::hasSamplerState(EGpuProgramType type, const FString &name) const {
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

bool FGpuParamsBase::hasParamBlock(EGpuProgramType type, const FString &name) const {
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

FGpuParamBlockDesc *FGpuParamsBase::getParamBlockDesc(EGpuProgramType type, const FString &name) const {
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

FGpuParamDataDesc *FGpuParamsBase::getParamDesc(EGpuProgramType type, const FString &name) const {
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

FGpuParams::FGpuParams(FGpuPipelineParamInfoBase *paramInfo) : FGpuParamsBase(paramInfo) {
    uint32_t paramBlockCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::ParamBlock);
    uint32_t buffersCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::Buffer);
    uint32_t textureCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::Texture);
    uint32_t samplerCount = mParamInfo->getElementCount(FGpuPipelineParamInfo::ParamType::SamplerState);

    auto paramBlocksSize = sizeof(ParamsBufferType) * paramBlockCount;
    auto buffersSize = sizeof(BufferType) * buffersCount;
    auto texturesSize = (sizeof(TextureType) + sizeof(FTextureSurface)) * textureCount;
    auto samplersSize = sizeof(BufferType) * samplerCount;

    auto totalSize = paramBlocksSize + buffersSize; // + texturesSize + samplersSize;

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

    /*data += buffersSize;
    mSampledTextureData = (TextureData *) data;
    for (uint32_t i = 0; i < textureCount; i++) {
        new (&mSampledTextureData[i].texture) TextureType();
        new (&mSampledTextureData[i].surface) FTextureSurface(0, 0, 0, 0);
    }

    data += texturesSize;
    mSamplerStates = (SamplerType *) data;
    for (uint32_t i = 0; i < samplerCount; i++) {
        new (&mSamplerStates[i]) SamplerType();
    }*/
}

FGpuParams::~FGpuParams() {
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

    free(mParamBlockBuffers);
}

template<class T>
void FGpuParams::getParam(EGpuProgramType type, const FString &name, FGpuDataParam<T> &output) const {
    const auto paramDescs = mParamInfo->getParamDesc(type);

    if (paramDescs == nullptr) {
        output = GpuDataParam<T>(nullptr, nullptr);
        LOG(FLogRenderAPI, Warning, TEXT("Cannot find parameter with the name: '{0}'"), *name);
        return;
    }

    auto it = paramDescs->params.find(name);
    if (it == nullptr) {
        output = GpuDataParam<T>(nullptr, nullptr);
        LOG(FLogRenderAPI, Warning, TEXT("Cannot find parameter with the name: '{0}'"), *name);
        return;
    } else {
        output = GpuDataParam<T>(*it, this);
    }
}

void FGpuParams::getBufferParam(EGpuProgramType type, const FString &name, FGpuParamBuffer &output) const {
    auto desc = mParamInfo->getParamDesc(type);
    if (desc == nullptr) {
        return;
    }

    auto it = desc->buffers.find(name);
    if (it == nullptr) {
        output = FGpuParamBuffer(nullptr, nullptr);
        LOG(FLogRenderAPI, Warning, TEXT("Cannot find buffer parameter with the name: '%ls'"), *name);
    } else {
        output = FGpuParamBuffer(&*it, const_cast<FGpuParams*>(this));
    }
}

FGpuParams::ParamsBufferType FGpuParams::getParamBlockBuffer(uint32_t set, uint32_t slot) const {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::ParamBlock, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return nullptr;
    }

    return mParamBlockBuffers[globalSlot];
}

FGpuParams::BufferType FGpuParams::getBuffer(uint32_t set, uint32_t slot) const {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::Buffer, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return nullptr;
    }

    return mBuffers[globalSlot];
}

void FGpuParams::setParamBlockBuffer(EGpuProgramType type, const FString &name,
                                     const FGpuParams::ParamsBufferType &paramBlockBuffer) {
    const auto &paramDescs = mParamInfo->getParamDesc(type);
    if (paramDescs == nullptr) {
        LOG(FLogRenderAPI, Warning, TEXT("Cannot find parameter block with the name: '%ls'"), *name);
        return;
    }

    auto it = paramDescs->paramBlocks.find(name);
    if (it == nullptr) {
        LOG(FLogRenderAPI, Warning, TEXT("Cannot find parameter block with the name: '%ls'"), *name);
        return;
    }

    setParamBlockBuffer((*it).set, (*it).slot, paramBlockBuffer);
}

void FGpuParams::setParamBlockBuffer(const FString &name, const FGpuParams::ParamsBufferType &paramBlockBuffer) {
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

void FGpuParams::setParamBlockBuffer(uint32_t set, uint32_t slot, const FGpuParams::ParamsBufferType &paramBlockBuffer) {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::ParamBlock, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return;
    }

    mParamBlockBuffers[globalSlot] = paramBlockBuffer;
}

void FGpuParams::setBuffer(uint32_t set, uint32_t slot, const FGpuParams::BufferType &buffer) {
    auto globalSlot = mParamInfo->getSequentialSlot(FGpuPipelineParamInfo::ParamType::Buffer, set, slot);
    if (globalSlot == static_cast<uint32_t>(-1)) {
        return;
    }

    mBuffers[globalSlot] = buffer;
}
