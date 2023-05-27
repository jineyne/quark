#pragma once

#include "CorePrerequisites.h"
#include "GpuProgram.h"
#include "Memory/GroupAllocator.h"

struct FGpuPipelineParamsDesc {
    GpuParamDesc *vertexParams = nullptr;
    GpuParamDesc *fragmentParams = nullptr;
    GpuParamDesc *geometryParams = nullptr;
    GpuParamDesc *hullParams = nullptr;
    GpuParamDesc *domainParams = nullptr;
    GpuParamDesc * computeParams = nullptr;
};

struct GpuParamBinding {
    uint32_t set = static_cast<uint32_t>(-1);
    uint32_t slot = static_cast<uint32_t>(-1);
};

class DLL_EXPORT FGpuPipelineParamInfoBase {
public:
    enum class ParamType {
        ParamBlock, Texture, LoadStoreTexture, Buffer, SamplerState, Count
    };

private:
    struct SetInfo {
        uint32_t *slotIndices;
        ParamType *slotTypes;
        uint32_t *slotSamplers;
        uint32_t slotCount;
    };

    struct ResourceInfo {
        uint32_t set;
        uint32_t slot;
    };

    std::array<GpuParamDesc *, 6> mParamDescList;
    GroupAllocator mAlloc;

    uint32_t mSetCount;
    uint32_t mElementCount;
    SetInfo *mSetInfoList;

    TArray<uint32_t> mElementCountPerType;
    TArray<ResourceInfo *> mResourceInfoList;

public:
    FGpuPipelineParamInfoBase(const FGpuPipelineParamsDesc &desc);
    virtual ~FGpuPipelineParamInfoBase() = default;

public:
    uint32_t getSequentialSlot(ParamType type, uint32_t set, uint32_t slot) const;

    void getBinding(ParamType type, uint32_t sequentialSlot, uint32_t &set, uint32_t &slot) const;
    void getBinding(EGpuProgramType program, ParamType type, const String &name, GpuParamBinding &binding);
    void getBindings(ParamType type, const String &name, GpuParamBinding(&bindings)[static_cast<uint32_t>(EGpuProgramType::Count)]);

    GpuParamDesc *getParamDesc(EGpuProgramType type) const { return mParamDescList[static_cast<uint32_t>(type)]; }

    uint32_t getSetCount() const { return mSetCount; }
    uint32_t getElementCount() const { return mElementCount; }
    uint32_t getElementCount(ParamType type) { return mElementCountPerType[static_cast<uint32_t>(type)]; }
};

class DLL_EXPORT FGpuPipelineParamInfo : public FGpuPipelineParamInfoBase {
public:
    FGpuPipelineParamInfo(const FGpuPipelineParamsDesc &desc);
    virtual ~FGpuPipelineParamInfo() = default;

public:
    static FGpuPipelineParamInfo *New(const FGpuPipelineParamsDesc &desc);
};