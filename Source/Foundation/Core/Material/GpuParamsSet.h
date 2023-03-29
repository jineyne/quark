#pragma once

#include "CorePrerequisites.h"
#include "MaterialParams.h"
#include "Shader.h"

class DLL_EXPORT FGpuParamsSet {
public:
    using GpuParamsType = FGpuParams;
    using MaterialParamsType = FMaterialParams;
    using ParamBlockPtrType = FGpuParamBlockBuffer *;
    using ParamBlockType = FGpuParamBlockBuffer;
    using TechniqueType = FTechnique;
    using ShaderType = FShader *;
    using GpuProgramPtrType = FGpuProgram *;
    using TextureType = FResourceHandle<FTexture>;
    using BufferType = FGpuBuffer *;
    using SamplerStateType = FSamplerState *;
    using GraphicsPipelineStateType = FGraphicsPipelineState;

    struct BlockBinding {
        uint32_t set;
        uint32_t slot;
    };

    struct PassBlockBindings {
        BlockBinding bindings[static_cast<uint32_t>(EGpuProgramType::Count)];
    };

    struct BlockInfo {
    public:
        FString name;
        uint32_t set;
        uint32_t slot;
        ParamBlockPtrType buffer;
        bool shareable;
        bool allowUpdate;
        bool isUsed;

        PassBlockBindings *passData;

        BlockInfo(FString name, uint32_t set, uint32_t slot, const ParamBlockPtrType &buffer, bool shareable)
                : name(std::move(name)), set(set), slot(slot), buffer(buffer), shareable(shareable)
                , allowUpdate(true), isUsed(true), passData(nullptr) {
        }

        inline BlockInfo() = default;
    };

    struct DataParamInfo {
        uint32_t paramIdx;
        uint32_t blockIdx;
        uint32_t offset;
        uint32_t arrayStride;
    };

    struct ObjectParamInfo {
        uint32_t paramIdx;
        uint32_t slotIdx;
        uint32_t setIdx;
    };

    struct StageParamInfo {
        ObjectParamInfo *textures;
        uint32_t textureCount;

        ObjectParamInfo *buffers;
        uint32_t bufferCount;

        ObjectParamInfo *samplerStates;
        uint32_t samplerCount;
    };

    struct PassParamInfo {
        std::array<StageParamInfo, static_cast<uint32_t>(EGpuProgramType::Count)> stages;
    };

private:
    TArray<GpuParamsType *> mPassParams;
    TArray<BlockInfo> mBlocks;
    TArray<DataParamInfo> mDataParamInfos;
    PassParamInfo *mPassParamInfos;

    uint64_t mParamVersion;
    uint8_t *mData;

public:
    FGpuParamsSet() = default;
    FGpuParamsSet(TechniqueType *technique, const ShaderType &shader, MaterialParamsType *params);

    ~FGpuParamsSet();

public:
    GpuParamsType *getGpuParams(uint32_t passIdx = 0);
    uint32_t getParamBlockBufferIndex(const FString &name);

    void setParamBlockBuffer(uint32_t index, ParamBlockPtrType paramBlock, bool ignoreInUpdate = false);
    void setParamBlockBuffer(const FString &name, ParamBlockPtrType paramBlock, bool ignoreInUpdate = false);

    uint32_t getPassesCount() const { return static_cast<uint32_t>(mPassParams.length()); }

    void update(MaterialParamsType *params, float t = 0.0f, bool updateAll = false);
};
