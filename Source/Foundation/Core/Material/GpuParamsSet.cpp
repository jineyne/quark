#include "GpuParamsSet.h"

#include "Material/Technique.h"
#include "Material/Pass.h"

#include "RenderAPI/GpuParamBlockBuffer.h"
#include "RenderAPI/GpuParams.h"
#include "RenderAPI/GpuProgram.h"

#include "Exception/Exception.h"
#include "RenderAPI/RenderAPI.h"

struct ValidParamKey {
    FString name;
    FMaterialParams::EParamType type;

    ValidParamKey(const FString &name, const FMaterialParams::EParamType &type)
            :name(name), type(type) {
    }

    bool operator== (const ValidParamKey &rhs) const {
        return name == rhs.name && type == rhs.type;
    }

    bool operator!= (const ValidParamKey &rhs) const {
        return !(*this == rhs);
    }
};

namespace std {
    /** Hash value generator for ValidParamKey. */
    template<>
    struct hash<ValidParamKey> {
        size_t operator()(const ValidParamKey &key) const {
            size_t hash = 0;
            CombineHash(hash, key.name);
            CombineHash(hash, key.type);

            return hash;
        }
    };
}

struct ShaderBlockDesc {
    FString name;
    EBufferUsage usage;
    int size;
    bool external;
    uint32_t sequentialIdx;
    uint32_t set;
    uint32_t slot;
};

TArray<FGpuParamDesc *> getAllParamDesc(FTechnique *technique) {
    TArray<FGpuParamDesc *> result;

    for (uint32_t i = 0; i < technique->getPassesCount(); i++) {
        auto pass = technique->getPass(i);
        const auto &graphicsPiepline = pass->getGraphicsPipelineState();

        if (graphicsPiepline) {
            auto vertProgram = graphicsPiepline->getVertexProgram();
            if (vertProgram) {
                result.add(vertProgram->getParamDesc());
            }

            auto fragProgram = graphicsPiepline->getFragmentProgram();
            if (fragProgram) {
                result.add(fragProgram->getParamDesc());
            }
        }
    }

    return result;
}

bool areParamsEquals(const FGpuParamDataDesc &paramA, const FGpuParamDataDesc &paramB, bool ignoreBufferOffsets) {
    bool equal = paramA.arraySize == paramB.arraySize && paramA.elementSize == paramB.elementSize
                 && paramA.type == paramB.type && paramA.arrayElementStride == paramB.arrayElementStride;

    if (!ignoreBufferOffsets)
        equal &= paramA.cpuMemOffset == paramB.cpuMemOffset && paramA.gpuMemOffset == paramB.gpuMemOffset;

    return equal;
}

TArray<ShaderBlockDesc> determineValidShareableParamBlocks(const TArray<FGpuParamDesc *> &paramDescs,
                                                           const TMap<FString, FShaderParamBlockDesc> &shaderDesc) {
    struct BlockInfo {
        FGpuParamBlockDesc *blockDesc;
        FGpuParamDesc *paramDesc;
        bool isValid;
        uint32_t set;
        uint32_t slot;

        BlockInfo() {}
        BlockInfo(FGpuParamBlockDesc *blockDesc, FGpuParamDesc *paramDesc, bool isValid = true)
                :blockDesc(blockDesc), paramDesc(paramDesc), isValid(isValid) {
        }
    };

    TMap<FString, BlockInfo> uniqueParamBlocks;

    for (auto paramIt = paramDescs.begin(); paramIt != paramDescs.end(); ++paramIt) {
        const FGpuParamDesc &desc = **paramIt;
        for (auto blockIt = desc.paramBlocks.begin(); blockIt != desc.paramBlocks.end(); ++blockIt) {
            const FGpuParamBlockDesc &block = blockIt->value;

            if (!block.isShareable) {
                continue;
            }

            auto it = uniqueParamBlocks.find(blockIt->key);
            if (it == nullptr) {
                uniqueParamBlocks[blockIt->key] = BlockInfo(const_cast<FGpuParamBlockDesc *>(&block), *paramIt);
                continue;
            }

            const auto &otherBlock = *(*it).blockDesc;

            if (!(*it).isValid) {
                continue;
            }

            FString otherBlockName = otherBlock.name;
            FGpuParamDesc *otherDesc = (*it).paramDesc;

            for (auto myIt = desc.params.begin(); myIt != desc.params.end(); ++myIt) {
                const auto &myParam = myIt->value;

                if (myParam.paramBlockSet != block.set || myParam.paramBlockSlot != block.slot) {
                    continue;
                }

                auto otherParamFind = otherDesc->params.find(myIt->key);

                if (otherParamFind == nullptr) {
                    break;
                }

                const auto &otherParam = (*otherParamFind);

                if (!areParamsEquals(myParam, otherParam, false) || block.name != otherBlockName) {
                    break;
                }
            }
        }
    }

    TArray<ShaderBlockDesc> output;
    for (auto &entry : uniqueParamBlocks) {
        if (!entry.value.isValid) {
            continue;
        }

        const auto &block = *entry.value.blockDesc;

        ShaderBlockDesc desc{};
        desc.external = false;
        desc.usage = EBufferUsage::Static;
        desc.size = block.blockSize * sizeof(uint32_t);
        desc.name = entry.key;
        desc.set = block.set;
        desc.slot = block.slot;

        auto it = shaderDesc.find(entry.key);
        if (it != nullptr) {
            desc.external = (*it).shared || (*it).rendererSemantic != TEXT("");
            desc.usage = (*it).usage;
        }

        output.add(desc);
    }

    return output;
}

TMap<FString, FGpuParamDataDesc *> determineValidDataParameters(const TArray<FGpuParamDesc *> &paramDescs) {
    TMap<FString, FGpuParamDataDesc *> foundDataParams;
    TMap<FString, bool> validParams;

    for (auto it = paramDescs.begin(); it != paramDescs.end(); ++it) {
        const auto &desc = **it;

        for (auto paramIt = desc.params.begin(); paramIt != desc.params.end(); ++paramIt) {
            const auto &param = paramIt->value;

            auto dataIt = validParams.find(paramIt->key);
            if (dataIt == nullptr) {
                validParams[paramIt->key] = true;
                foundDataParams[paramIt->key] = const_cast<FGpuParamDataDesc *>(&param);
            } else {
                if (validParams[paramIt->key]) {
                    auto dataIt2 = foundDataParams.find(paramIt->key);
                    const auto *otherParam = (*dataIt2);
                    if (!areParamsEquals(param, *otherParam, true)) {
                        validParams[paramIt->key] = false;
                        foundDataParams.remove(paramIt->key);
                    }
                }
            }
        }
    }

    return foundDataParams;
}

TArray<FGpuParamObjectDesc *> determineValidObjectParameters(const TArray<FGpuParamDesc *> &paramDescs) {
    TArray<FGpuParamObjectDesc *> validParams;

    for (auto it = paramDescs.begin(); it != paramDescs.end(); ++it) {
        const auto &desc = **it;

        for (auto samplerIt = desc.samplers.begin(); samplerIt != desc.samplers.end(); ++samplerIt) {
            validParams.add(const_cast<FGpuParamObjectDesc *>(&(samplerIt->value)));
        }

        for (auto textureIt = desc.textures.begin(); textureIt != desc.textures.end(); ++textureIt) {
            validParams.add(const_cast<FGpuParamObjectDesc *>(&(textureIt->value)));
        }

        for (auto bufferIt = desc.buffers.begin(); bufferIt != desc.buffers.end(); ++bufferIt) {
            validParams.add(const_cast<FGpuParamObjectDesc *>(&(bufferIt->value)));
        }
    }

    return validParams;
}

TMap<FString, FString> determineParameterToBlockMapping(const TArray<FGpuParamDesc *> &paramDescs) {
    TMap<FString, FString> paramToParamBlock;

    for (auto descIt = paramDescs.begin(); descIt != paramDescs.end(); ++descIt) {
        const auto &desc = **descIt;
        for (auto paramIt = desc.params.begin(); paramIt != desc.params.end(); ++paramIt) {
            const auto &param = paramIt->value;

            auto it = paramToParamBlock.find(param.name);
            if (it != nullptr) {
                continue;
            }

            for (auto blockIt = desc.paramBlocks.begin(); blockIt != desc.paramBlocks.end(); ++blockIt) {
                auto &block = blockIt->value;
                if (block.set == param.paramBlockSet && block.slot == param.paramBlockSlot) {
                    paramToParamBlock[param.name] = block.name;
                    break;
                }
            }
        }
    }

    return paramToParamBlock;
}

TMap<ValidParamKey, FString> determineValidParameters(const TArray<FGpuParamDesc *> &paramDescs,
                                                      const TMap<FString, FShaderDataParamDesc> &dataParams,
                                                      const TMap<FString, FShaderObjectParamDesc> &textureParams,
                                                      const TMap<FString, FShaderObjectParamDesc> &bufferParams,
                                                      const TMap<FString, FShaderObjectParamDesc> &samplerParams) {
    TMap<ValidParamKey, FString> validParams;

    auto validDataParameters = determineValidDataParameters(paramDescs);
    auto validObjectParameters = determineValidObjectParameters(paramDescs);
    auto paramToParamBlockMap = determineParameterToBlockMapping(paramDescs);

    for (auto paramIt = dataParams.begin(); paramIt != dataParams.end(); ++paramIt) {
        auto it = validDataParameters.find(paramIt->value.gpuVariableName);

        if (it == nullptr) {
            continue;
        }

        if ((*it)->type != paramIt->value.type && !(paramIt->value.type == EGpuParamDataType::Color && ((*it)->type == EGpuParamDataType::Float4 || (*it)->type == EGpuParamDataType::Float3))) {

            LOG(FLogMaterial, Warning, TEXT("Ignoring shader parameter \"%ls\". Type doesn't match the one defined in the "
                                      "GPU program. Shader defined type: %ld - Gpu program defined type: %ld"),
                *paramIt->key, static_cast<uint32_t>(paramIt->value.type), static_cast<uint32_t>((*it)->type));

            continue;
        }

        auto blockIt = paramToParamBlockMap.find(paramIt->value.gpuVariableName);

        if (blockIt == nullptr) {
            EXCEPT(FLogMaterial, InternalErrorException, TEXT("Parameter doesn't exist in param to param block TMap but exists in valid param TMap."));
        }

        ValidParamKey key(paramIt->value.gpuVariableName, FMaterialParams::EParamType::Data);
        validParams.add(key, paramIt->key);
    }

    auto determineObjectMappings = [&](const TMap<FString, FShaderObjectParamDesc> &params, FMaterialParams::EParamType paramType) {
        for (const auto &param : params) {
            const auto &gpuVariableNames = param.value.gpuVariableNames;
            for (const auto &gpuVariableName : gpuVariableNames) {
                for (auto &validObjectParameter : validObjectParameters) {
                    if (validObjectParameter->name == gpuVariableName) {
                        ValidParamKey key(gpuVariableName, paramType);
                        validParams.add(key, param.key);

                        break;
                    }
                }
            }
        }
    };

    determineObjectMappings(textureParams, FMaterialParams::EParamType::Texture);
    determineObjectMappings(samplerParams, FMaterialParams::EParamType::Sampler);
    determineObjectMappings(bufferParams, FMaterialParams::EParamType::Buffer);

    return validParams;
}

FGpuParamsSet::FGpuParamsSet(FGpuParamsSet::TechniqueType *technique, FGpuParamsSet::ShaderType const &shader,
                             FGpuParamsSet::MaterialParamsType *params)
        : mPassParams(technique->getPassesCount()), mParamVersion(0) {
    auto passesCount = technique->getPassesCount();

    for (uint32_t i = 0; i < passesCount; i++) {
        const auto pass = technique->getPass(i);
        const auto gfxPipeline = pass->getGraphicsPipelineState();
        if (gfxPipeline != nullptr) {
            mPassParams[i] = FGpuParams::New(gfxPipeline);
        } else {
            // TODO: Compute pipeline
            EXCEPT(FLogMaterial, NotImplementedException, TEXT("There is not gfx pipeline."));
        }
    }

    auto allParamDescs = getAllParamDesc(technique);
    auto paramBlockData = determineValidShareableParamBlocks(allParamDescs, shader->getParamBlocks());
    auto validParams = determineValidParameters(allParamDescs, shader->getDataParams(), shader->getTextureParams(),
                                                shader->getBufferParams(), shader->getSamplerParams());

    TMap<FString, ParamBlockPtrType> paramBlockBuffers;

    for (auto &paramBlock : paramBlockData) {
        ParamBlockPtrType newParamBlockBuffer = nullptr;
        if (!paramBlock.external) {
            newParamBlockBuffer = ParamBlockType::New(paramBlock.size, paramBlock.usage);
        }

        paramBlock.sequentialIdx = static_cast<uint32_t>(mBlocks.length());
        paramBlockBuffers[paramBlock.name] = newParamBlockBuffer;
        mBlocks.emplace(paramBlock.name, paramBlock.set, paramBlock.slot, newParamBlockBuffer, true);
    }

    assert(passesCount < 64);

    for (uint32_t i = 0; i < passesCount; i++) {
        auto paramPtr = mPassParams[i];
        for (uint32_t j = 0; j < static_cast<uint32_t>(EGpuProgramType::Count); j++) {
            auto progType = static_cast<EGpuProgramType>(j);

            for (auto &block : paramBlockData) {
                const FString &paramBlockName = block.name;
                if (paramPtr->hasParamBlock(progType, paramBlockName)) {
                    auto blockBuffer = paramBlockBuffers[paramBlockName];
                    paramPtr->setParamBlockBuffer(progType, paramBlockName, blockBuffer);
                }
            }

            auto desc = paramPtr->getParamDesc(progType);
            if (desc == nullptr) {
                continue;
            }

            for (auto blockIt = desc->paramBlocks.begin(); blockIt != desc->paramBlocks.end(); ++blockIt) {
                const auto &blockDesc = blockIt->value;
                auto globalBlockIdx = static_cast<uint32_t>(-1);
                if (!blockDesc.isShareable) {
                    auto newParamBlockBuffer = ParamBlockType::New(blockDesc.blockSize * sizeof(uint32_t));
                    globalBlockIdx = static_cast<uint32_t>(mBlocks.length());
                    paramPtr->setParamBlockBuffer(progType, blockIt->key, newParamBlockBuffer);
                    mBlocks.emplace(blockIt->key, blockIt->value.set, blockIt->value.slot, newParamBlockBuffer, false);
                } else {
                    auto it = std::find_if(paramBlockData.begin(), paramBlockData.end(), [&](const ShaderBlockDesc &x) {
                        return x.name == blockIt->key;
                    });

                    if (it != paramBlockData.end()) {
                        globalBlockIdx = it->sequentialIdx;
                    }
                }

                if (globalBlockIdx == static_cast<uint32_t>(-1)) {
                    continue;
                }

                for (auto &dataParam : desc->params) {
                    if (dataParam.value.paramBlockSet != blockDesc.set ||
                        dataParam.value.paramBlockSlot != blockDesc.slot) {
                        continue;
                    }

                    ValidParamKey key(dataParam.key, FMaterialParams::EParamType::Data);
                    auto it = validParams.find(key);
                    if (it == nullptr) {
                        continue;
                    }

                    auto paramIdx = params->getParamIndex(*it);
                    assert(paramIdx != static_cast<uint32_t>(-1));

                    mDataParamInfos.add(DataParamInfo());
                    auto &paramInfo = mDataParamInfos.top();

                    paramInfo.paramIdx = paramIdx;
                    paramInfo.blockIdx = globalBlockIdx;
                    paramInfo.offset = dataParam.value.cpuMemOffset;
                    paramInfo.arrayStride = dataParam.value.arrayElementStride;
                }
            }
        }
    }

    auto &allParamBlocks = shader->getParamBlocks();
    for (auto &entry : allParamBlocks) {
        auto it = std::find_if(mBlocks.begin(), mBlocks.end(), [&](const FGpuParamsSet::BlockInfo &x) {
            return x.name == entry.key;
        });

        if (it == mBlocks.end()) {
            mBlocks.add(BlockInfo(entry.key, 0, 0, nullptr, true));
            mBlocks.top().isUsed = false;
        }
    }

    // 개체 매개변수에 대한 정보를 생성한다.
    TArray<ObjectParamInfo> objParamInfos;

    uint32_t offsetsSize = passesCount * static_cast<uint32_t>(EGpuProgramType::Count) * 4 * sizeof(uint32_t);
    TArray<uint32_t> offsets(offsetsSize);
    std::fill(offsets.begin(), offsets.end(), 0);

    // 이 중 몇개를 쓸지 모르기 때문에 먼저 모든 개체를 임시 배열에 저장한다.
    uint32_t totalObjectsCount = 0;
    auto stageOffsets = offsets.begin();
    for (uint32_t i = 0; i < passesCount; i++) {
        auto paramPtr = mPassParams[i];
        for (uint32_t j = 0; j < static_cast<uint32_t>(EGpuProgramType::Count); j++) {
            auto progType = static_cast<EGpuProgramType>(j);

            auto processObjectParams = [&](const TMap<FString, FGpuParamObjectDesc> &gpuParams, uint32_t stageIdx,
                                           FMaterialParams::EParamType paramType) {
                for (auto &param : gpuParams) {
                    ValidParamKey key(param.key, paramType);

                    auto it = validParams.find(key);
                    if (it == nullptr) {
                        continue;
                    }

                    uint32_t paramIdx;
                    auto result = params->getParamIndex(*it, paramType, EGpuParamDataType::Unknown, 0, paramIdx);

                    assert(result == FMaterialParams::EGetParamResult::Success);

                    objParamInfos.add(ObjectParamInfo());
                    auto &paramInfo = objParamInfos.top();
                    paramInfo.paramIdx = paramIdx;
                    paramInfo.slotIdx = param.value.slot;
                    paramInfo.setIdx = param.value.set;

                    stageOffsets[stageIdx]++;
                    totalObjectsCount++;
                }
            };

            auto desc = paramPtr->getParamDesc(progType);
            if (desc == nullptr) {
                stageOffsets += 4;
                continue;
            }

            processObjectParams(desc->textures, 0, FMaterialParams::EParamType::Texture);
            processObjectParams(desc->buffers, 2, FMaterialParams::EParamType::Buffer);
            processObjectParams(desc->samplers, 3, FMaterialParams::EParamType::Sampler);

            stageOffsets += 4;
        }
    }

    // 모든 개체를 영구 저장소에 전송한다.
    auto blockCount = static_cast<uint32_t>(mBlocks.length());
    auto blockBindingsSize = blockCount * passesCount * sizeof(PassBlockBindings);
    auto objectParamInfosSize = totalObjectsCount * sizeof(ObjectParamInfo) + passesCount * sizeof(PassParamInfo);

    mData = q_alloc<uint8_t>(objectParamInfosSize + blockBindingsSize);
    uint8_t *dataIter = mData;

    mPassParamInfos = reinterpret_cast<PassParamInfo *>(dataIter);
    std::memset(mPassParamInfos, 0, objectParamInfosSize);
    dataIter += objectParamInfosSize;

    auto stageInfos = reinterpret_cast<StageParamInfo *>(mPassParamInfos);
    auto objInfos = reinterpret_cast<ObjectParamInfo *>(mPassParamInfos + passesCount);
    std::memcpy(objInfos, *objParamInfos, totalObjectsCount * sizeof(ObjectParamInfo));

    uint32_t objInfoOffset = 0;
    stageOffsets = offsets.begin();
    for (uint32_t i = 0; i < passesCount; i++) {
        for (uint32_t j = 0; j < static_cast<uint32_t>(EGpuProgramType::Count); j++) {
            auto &stage = stageInfos[i * static_cast<uint32_t>(EGpuProgramType::Count) + j];

            if (stageOffsets[0] > 0) {
                uint32_t entriesCount = stageOffsets[0];

                stage.textures = objInfos + objInfoOffset;
                stage.textureCount = entriesCount;

                objInfoOffset += entriesCount;
            }

            // 1 -> loadStoreTexture

            if (stageOffsets[2] > 0) {
                uint32_t entriesCount = stageOffsets[2];

                stage.buffers = objInfos + objInfoOffset;
                stage.bufferCount = entriesCount;

                objInfoOffset += entriesCount;
            }

            if (stageOffsets[3] > 0) {
                uint32_t entriesCount = stageOffsets[3];

                stage.samplerStates = objInfos + objInfoOffset;
                stage.samplerCount = entriesCount;

                objInfoOffset += entriesCount;
            }

            stageOffsets += 4;
        }
    }

    for (auto &block : mBlocks) {
        block.passData = reinterpret_cast<PassBlockBindings *>(dataIter);
        dataIter += sizeof(PassBlockBindings) * passesCount;
    }

    for (auto &block : mBlocks) {
        for (uint32_t i = 0; i < passesCount; i++) {
            auto paramPtr = mPassParams[i];

            for (uint32_t j = 0; j < static_cast<uint32_t>(EGpuProgramType::Count); j++) {
                auto progType = static_cast<EGpuProgramType>(j);

                auto desc = paramPtr->getParamDesc(progType);
                if (desc == nullptr) {
                    block.passData[i].bindings[j].set = -1;
                    block.passData[i].bindings[j].slot = -1;

                    continue;
                }

                auto it = desc->paramBlocks.find(block.name);
                if (it == nullptr) {
                    block.passData[i].bindings[j].set = -1;
                    block.passData[i].bindings[j].slot = -1;

                    continue;
                }

                block.passData[i].bindings[j].set = (*it).set;
                block.passData[i].bindings[j].slot = (*it).slot;
            }
        }
    }
}

FGpuParamsSet::~FGpuParamsSet() {
    q_free(mData);
}

FGpuParamsSet::GpuParamsType *FGpuParamsSet::getGpuParams(uint32_t passIdx) {
    if (passIdx >= mPassParams.length()) {
        return nullptr;
    }

    return mPassParams[passIdx];
}

uint32_t FGpuParamsSet::getParamBlockBufferIndex(const FString &name) {
    for (uint32_t i = 0; i < static_cast<uint32_t>(mBlocks.length()); i++) {
        const auto &block = mBlocks[i];
        if (block.name == name) {
            return i;
        }
    }

    return -1;
}

void FGpuParamsSet::setParamBlockBuffer(uint32_t index, FGpuParamsSet::ParamBlockPtrType paramBlock, bool ignoreInUpdate) {
    auto &blockInfo = mBlocks[index];
    if (!blockInfo.shareable) {
        LOG(FLogRenderAPI, Error,
            TEXT("Cannot set parameter block buffer with the name \"{0}\". Buffer is not assignable. "),
            *blockInfo.name);
        return;
    }

    if (!blockInfo.isUsed) {
        return;
    }

    blockInfo.allowUpdate = !ignoreInUpdate;

    if (blockInfo.buffer != paramBlock) {
        blockInfo.buffer = paramBlock;

        auto passesCount = static_cast<uint32_t>(mPassParams.length());
        for (uint32_t j = 0; j < passesCount; j++) {
            auto paramPtr = mPassParams[j];
            for (uint32_t i = 0; i < static_cast<uint32_t>(EGpuProgramType::Count); i++) {
                auto progType = static_cast<EGpuProgramType>(i);

                const auto &binding = blockInfo.passData[j].bindings[i];
                if (binding.slot != static_cast<uint32_t>(-1)) {
                    paramPtr->setParamBlockBuffer(binding.set, binding.slot, paramBlock);
                }
            }
        }
    }
}

void FGpuParamsSet::setParamBlockBuffer(const FString &name, FGpuParamsSet::ParamBlockPtrType paramBlock,
                                        bool ignoreInUpdate) {
    auto bufferIdx = getParamBlockBufferIndex(name);
    if (bufferIdx == static_cast<uint32_t>(-1)) {
        LOG(FLogMaterial, Error, TEXT("Cannot set parameter block buffer with the name \"%ls\". Buffer name not found. "), *name);
        return;
    }

    setParamBlockBuffer(bufferIdx, paramBlock, ignoreInUpdate);
}

void FGpuParamsSet::update(FGpuParamsSet::MaterialParamsType *params, float t, bool updateAll) {
    for (auto &paramInfo : mDataParamInfos) {
        auto paramBlock = mBlocks[paramInfo.blockIdx].buffer;
        if (paramBlock == nullptr || !mBlocks[paramInfo.blockIdx].allowUpdate) {
            continue;
        }

        const auto *materialParamInfo = params->getParamData(paramInfo.paramIdx);
        uint32_t arraySize = materialParamInfo->arraySize == 0 ? 1 : materialParamInfo->arraySize;

        if (materialParamInfo->version <= mParamVersion && !updateAll) {
            continue;
        }

        if (materialParamInfo->dataType != EGpuParamDataType::Struct) {
            const auto &typeInfo = FGpuParams::ParamSizes.lookup[static_cast<int>(materialParamInfo->dataType)];
            uint32_t paramSize;

            if (materialParamInfo->dataType != EGpuParamDataType::Color) {
                paramSize = typeInfo.numColumns * typeInfo.numRows * typeInfo.baseTypeSize;
            } else {
                paramSize = paramInfo.arrayStride * typeInfo.baseTypeSize;
            }

            auto data = const_cast<uint8_t *>(params->getData(materialParamInfo->index));

            //TODO: check transpose
            for (uint32_t i = 0; i < arraySize; i++) {
                uint32_t readOffset = i * paramSize;
                uint32_t writeOffset = (paramInfo.offset + paramInfo.arrayStride * i) * sizeof(uint32_t);
                paramBlock->write(writeOffset, data + readOffset, paramSize);
            }
        } else {
            auto paramSize = params->getStructSize(*materialParamInfo);
            auto paramData = q_alloc<uint8_t>(paramSize);
            for (uint32_t i = 0; i < arraySize; i++) {
                params->getStructData(*materialParamInfo, paramData, paramSize, i);

                uint32_t writeOffset = (paramInfo.offset + paramInfo.arrayStride * i) * sizeof(uint32_t);
                paramBlock->write(writeOffset, paramData, paramSize);
            }
            q_free(paramData);
        }
    }

    const auto passesCount = static_cast<uint32_t>(mPassParams.length());
    for (uint32_t i = 0; i < passesCount; i++) {
        auto paramPtr = mPassParams[i];

        for (uint32_t j = 0; j < static_cast<uint32_t>(EGpuProgramType::Count); j++) {
            const auto &stageInfo = mPassParamInfos[i].stages[j];

            for (uint32_t textureIdx = 0; textureIdx < stageInfo.textureCount; textureIdx++) {
                const auto &paramInfo = stageInfo.textures[textureIdx];
                const auto materialParamInfo = params->getParamData(paramInfo.paramIdx);
                if (materialParamInfo->version <= mParamVersion && !updateAll) {
                    continue;
                }

                FTextureSurface surface;
                TextureType texture;

                params->getTexture(*materialParamInfo, texture, surface);
                paramPtr->setTexture(paramInfo.setIdx, paramInfo.slotIdx, texture, surface);
            }

            for (uint32_t bufferIdx = 0; bufferIdx < stageInfo.bufferCount; bufferIdx++) {
                const auto &paramInfo = stageInfo.buffers[bufferIdx];
                const auto materialParamInfo = params->getParamData(paramInfo.paramIdx);
                if (materialParamInfo->version <= mParamVersion && !updateAll) {
                    continue;
                }

                BufferType buffer;

                params->getBuffer(*materialParamInfo, buffer);
                paramPtr->setBuffer(paramInfo.setIdx, paramInfo.slotIdx, buffer);
            }

            for (uint32_t samplerIdx = 0; samplerIdx < stageInfo.samplerCount; samplerIdx++) {
                const auto &paramInfo = stageInfo.samplerStates[samplerIdx];
                const auto materialParamInfo = params->getParamData(paramInfo.paramIdx);
                if (materialParamInfo->version <= mParamVersion && !updateAll) {
                    continue;
                }

                SamplerStateType samplerStates;

                params->getSamplerState(*materialParamInfo, samplerStates);
                paramPtr->setSamplerState(paramInfo.setIdx, paramInfo.slotIdx, samplerStates);
            }
        }
    }

    mParamVersion = params->getParamVersion();
}
