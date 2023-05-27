#include "GpuPipelineParamInfo.h"
#include "Manager/RenderStateManager.h"
#include "RenderAPI.h"

FGpuPipelineParamInfoBase::FGpuPipelineParamInfoBase(const FGpuPipelineParamsDesc &desc)
        : mElementCountPerType(static_cast<int32_t>(ParamType::Count))
        , mResourceInfoList(static_cast<int32_t>(ParamType::Count))
        , mSetCount(0)
        , mElementCount(0) {
    std::fill(mElementCountPerType.begin(), mElementCountPerType.end(), 0);

    mParamDescList[static_cast<uint32_t>(EGpuProgramType::Vertex)] = desc.vertexParams;
    mParamDescList[static_cast<uint32_t>(EGpuProgramType::Fragment)] = desc.fragmentParams;
    mParamDescList[static_cast<uint32_t>(EGpuProgramType::Geometry)] = desc.geometryParams;
    mParamDescList[static_cast<uint32_t>(EGpuProgramType::Hull)] = desc.hullParams;
    mParamDescList[static_cast<uint32_t>(EGpuProgramType::Domain)] = desc.domainParams;
    mParamDescList[static_cast<uint32_t>(EGpuProgramType::Compute)] = desc.computeParams;

    for (uint32_t i = 0; i < mParamDescList.size(); i++) {
        auto paramDesc = mParamDescList[i];
        if (paramDesc == nullptr) {
            continue;
        }

        auto countBlockElements = [&](GpuParamBlockDesc &entry, ParamType type) {
            int idx = static_cast<int>(type);
            if ((entry.set + 1) > mSetCount) {
                mSetCount = entry.set + 1;
            }

            mElementCountPerType[idx]++;
            mElementCount++;
        };

        auto countObjectElements = [&](GpuParamObjectDesc &entry, ParamType type) {
            int idx = static_cast<int>(type);
            if ((entry.set + 1) > mSetCount) {
                mSetCount = entry.set + 1;
            }

            mElementCountPerType[idx]++;
            mElementCount++;
        };

        for (auto &paramBlock : paramDesc->paramBlocks) {
            countBlockElements(paramBlock.value, ParamType::ParamBlock);
        }

        for (auto &texture : paramDesc->textures) {
            countObjectElements(texture.value, ParamType::Texture);
        }

        for (auto &texture : paramDesc->loadStoreTextures) {
            countObjectElements(texture.value, ParamType::LoadStoreTexture);
        }

        for (auto &buffer : paramDesc->buffers) {
            countObjectElements(buffer.value, ParamType::Buffer);
        }

        for (auto &sampler : paramDesc->samplers) {
            countObjectElements(sampler.value, ParamType::SamplerState);
        }
    }

    TArray<uint32_t> slotCountPerSet(mSetCount);
    std::fill(slotCountPerSet.begin(), slotCountPerSet.end(), 0);

    for (uint32_t i = 0; i < mParamDescList.size(); i++) {
        auto paramDesc = mParamDescList[i];
        if (paramDesc == nullptr) {
            continue;
        }

        for (auto &paramBlocks : paramDesc->paramBlocks) {
            slotCountPerSet[paramBlocks.value.set] = std::max(slotCountPerSet[paramBlocks.value.set], paramBlocks.value.slot + 1);
        }

        for (auto &texture : paramDesc->textures) {
            slotCountPerSet[texture.value.set] = std::max(slotCountPerSet[texture.value.set], texture.value.slot + 1);
        }

        for (auto &texture : paramDesc->loadStoreTextures) {
            slotCountPerSet[texture.value.set] = std::max(slotCountPerSet[texture.value.set], texture.value.slot + 1);
        }

        for (auto &buffer : paramDesc->buffers) {
            slotCountPerSet[buffer.value.set] = std::max(slotCountPerSet[buffer.value.set], buffer.value.slot + 1);
        }

        for (auto &sampler : paramDesc->samplers) {
            slotCountPerSet[sampler.value.set] = std::max(slotCountPerSet[sampler.value.set], sampler.value.set + 1);
        }
    }

    uint32_t totalSlotCount = 0;
    for (uint32_t i = 0; i < mSetCount; i++) {
        totalSlotCount += slotCountPerSet[i];
    }

    mAlloc.reserve<SetInfo>(mSetCount).reserve<uint32_t>(totalSlotCount).reserve<ParamType>(totalSlotCount)
            .reserve<uint32_t>(totalSlotCount);

    for (uint32_t i = 0; i < static_cast<uint32_t>(ParamType::Count); i++)
        mAlloc.reserve<ResourceInfo>(mElementCountPerType[i]);

    mAlloc.init();

    mSetInfoList = mAlloc.alloc<SetInfo>(mSetCount);
    std::memset(mSetInfoList, 0, mSetCount * sizeof(SetInfo));

    for (uint32_t i = 0; i < mSetCount; i++) {
        mSetInfoList[i].slotCount = slotCountPerSet[i];
    }

    slotCountPerSet.clear();

    for (uint32_t i = 0; i < mSetCount; i++) {
        mSetInfoList[i].slotIndices = mAlloc.alloc<uint32_t>(mSetInfoList[i].slotCount);
        std::memset(mSetInfoList[i].slotIndices, -1, mSetInfoList[i].slotCount * sizeof(uint32_t));

        mSetInfoList[i].slotTypes = mAlloc.alloc<ParamType>(mSetInfoList[i].slotCount);

        mSetInfoList[i].slotSamplers = mAlloc.alloc<uint32_t>(mSetInfoList[i].slotCount);
        std::memset(mSetInfoList[i].slotSamplers, -1, mSetInfoList[i].slotCount * sizeof(uint32_t));
    }

    for (uint32_t i = 0; i < static_cast<uint32_t>(ParamType::Count); i++) {
        mResourceInfoList[i] = mAlloc.alloc<ResourceInfo>(mElementCountPerType[i]);
        mElementCountPerType[i] = 0;
    }

    auto populateBlockSetInfo = [&](GpuParamBlockDesc &entry, ParamType type) {
        int idx = static_cast<int>(type);

        auto sequentialIdx = mElementCountPerType[idx];

        auto &setInfo = mSetInfoList[entry.set];
        setInfo.slotIndices[entry.slot] = sequentialIdx;
        setInfo.slotTypes[entry.slot] = type;

        mResourceInfoList[idx][sequentialIdx].set = entry.set;
        mResourceInfoList[idx][sequentialIdx].slot = entry.slot;

        mElementCountPerType[idx]++;
    };

    auto populateObjectSetInfo = [&](GpuParamObjectDesc &entry, ParamType type) {
        int idx = static_cast<int>(type);

        auto sequentialIdx = mElementCountPerType[idx];

        auto &setInfo = mSetInfoList[entry.set];
        setInfo.slotIndices[entry.slot] = sequentialIdx;
        setInfo.slotTypes[entry.slot] = type;

        mResourceInfoList[idx][sequentialIdx].set = entry.set;
        mResourceInfoList[idx][sequentialIdx].slot = entry.slot;

        mElementCountPerType[idx]++;
    };

    for (uint32_t i = 0; i < mParamDescList.size(); i++) {
        auto paramDesc = mParamDescList[i];
        if (paramDesc == nullptr) {
            continue;
        }

        for (auto &paramBlock : paramDesc->paramBlocks) {
            populateBlockSetInfo(paramBlock.value, ParamType::ParamBlock);
        }

        for (auto &texture : paramDesc->textures) {
            populateObjectSetInfo(texture.value, ParamType::Texture);
        }

        for (auto &texture : paramDesc->loadStoreTextures) {
            populateObjectSetInfo(texture.value, ParamType::LoadStoreTexture);
        }

        for (auto &buffer : paramDesc->buffers) {
            populateObjectSetInfo(buffer.value, ParamType::Buffer);
        }

        {
            int idx = static_cast<int>(ParamType::SamplerState);
            for (auto &entry : paramDesc->samplers) {
                auto samplerDesc = entry.value;
                auto sequentialIdx = mElementCountPerType[idx];

                auto &setInfo = mSetInfoList[samplerDesc.set];
                if (setInfo.slotIndices[samplerDesc.slot] == static_cast<uint32_t>(-1)) {
                    setInfo.slotIndices[samplerDesc.slot] = sequentialIdx;
                    setInfo.slotTypes[samplerDesc.slot] = ParamType::SamplerState;
                } else {
                    setInfo.slotSamplers[samplerDesc.slot] = sequentialIdx;
                }

                mResourceInfoList[idx][sequentialIdx].set = samplerDesc.set;
                mResourceInfoList[idx][sequentialIdx].slot = samplerDesc.slot;

                mElementCountPerType[idx]++;
            }
        }
    }
}

uint32_t FGpuPipelineParamInfoBase::getSequentialSlot(FGpuPipelineParamInfoBase::ParamType type, uint32_t set,
                                                      uint32_t slot) const {
#if DEBUG_MODE
    if (set >= mSetCount) {
        LOG(LogRenderAPI, Error, TEXT("Set gIBO out of range: Valid range: [0, %ld). Requested: %ld."), mSetCount, set);

        return -1;
    }

    if (slot >= mSetInfoList[set].slotCount) {
        LOG(LogRenderAPI, Error, TEXT("Set gIBO out of range: Valid range: [0, %ld). Requested: %ld."), mSetInfoList[set].slotCount, set);

        return -1;
    }

    auto slotType = mSetInfoList[set].slotTypes[slot];
    if (slotType != type) {
        if (type == ParamType::SamplerState) {
            if (mSetInfoList[set].slotSamplers[slot] != static_cast<uint32_t>(-1)) {
                return mSetInfoList[set].slotSamplers[slot];
            }
        }

        LOG(LogRenderAPI, Error, TEXT("Requested parameter is not of the valid type. Requested: %ld. Actual: %ld."),
            static_cast<uint32_t>(type), static_cast<uint32_t>(mSetInfoList[set].slotTypes[slot]));

        return -1;
    }
#endif

    return mSetInfoList[set].slotIndices[slot];
}

void
FGpuPipelineParamInfoBase::getBinding(FGpuPipelineParamInfoBase::ParamType type, uint32_t sequentialSlot, uint32_t &set,
                                      uint32_t &slot) const {
#if DEBUG_MODE
    if (sequentialSlot >= mElementCountPerType[static_cast<int>(type)]) {
        LOG(LogRenderAPI, Error, TEXT("Sequential slot gIBO out of range: Valid range: [0, %ld). Requested: %ld."), static_cast<uint32_t>(type), sequentialSlot);

        set = 0;
        slot = 0;

        return;
    }
#endif

    set = mResourceInfoList[static_cast<int>(type)][sequentialSlot].set;
    slot = mResourceInfoList[static_cast<int>(type)][sequentialSlot].slot;
}

void FGpuPipelineParamInfoBase::getBinding(EGpuProgramType program, FGpuPipelineParamInfoBase::ParamType type,
                                           const String &name, GpuParamBinding &binding) {
    auto findBlock = [](GpuParamDesc::GpuParamBlockDescMap &map, const String &name, GpuParamBinding &binding) {
        auto it = map.find(name);
        if (it != nullptr) {
            binding.set = (*it).set;
            binding.slot = (*it).slot;
        } else {
            binding.set = binding.slot = static_cast<uint32_t>(-1);
        }
    };

    auto findObject = [](GpuParamDesc::GpuParamObjectDescMap &map, const String &name, GpuParamBinding &binding) {
        auto it = map.find(name);
        if (it != nullptr) {
            binding.set = (*it).set;
            binding.slot = (*it).slot;
        } else {
            binding.set = binding.slot = static_cast<uint32_t>(-1);
        }
    };

    auto paramDesc = mParamDescList[static_cast<uint32_t>(program)];
    if (paramDesc == nullptr) {
        binding.set = binding.slot = static_cast<uint32_t>(-1);
        return;
    }

    switch (type) {
        case ParamType::ParamBlock:
            findBlock(paramDesc->paramBlocks, name, binding);
            break;

        case ParamType::Texture:
            findObject(paramDesc->textures, name, binding);
            break;

        case ParamType::LoadStoreTexture:
            findObject(paramDesc->loadStoreTextures, name, binding);
            break;

        case ParamType::Buffer:
            findObject(paramDesc->buffers, name, binding);
            break;

        case ParamType::SamplerState:
            findObject(paramDesc->samplers, name, binding);
            break;

        default:
            break;
    }
}

void FGpuPipelineParamInfoBase::getBindings(FGpuPipelineParamInfoBase::ParamType type, const String &name,
                                            GpuParamBinding (&bindings)[static_cast<uint32_t>(EGpuProgramType::Count)]) {
    constexpr uint32_t paramDescSize =  sizeof(mParamDescList) / sizeof(mParamDescList[0]);

    static_assert(paramDescSize == static_cast<uint32_t>(EGpuProgramType::Count),
                  TEXT("Number of param descriptor structures must match the number of GPU program stages."));

    for (uint32_t i = 0; i < paramDescSize; ++i) {
        getBinding(static_cast<EGpuProgramType>(i), type, name, bindings[i]);
    }
}

FGpuPipelineParamInfo::FGpuPipelineParamInfo(const FGpuPipelineParamsDesc &desc) : FGpuPipelineParamInfoBase(desc) {

}

FGpuPipelineParamInfo *FGpuPipelineParamInfo::New(const FGpuPipelineParamsDesc &desc) {
    return RenderStateManager::Instance().createPipelineParamInfo(desc);
}
