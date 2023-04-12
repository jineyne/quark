#pragma once
#include "CorePrerequisites.h"

#include "Exception/Exception.h"
#include "RenderAPI/GpuParamDesc.h"
#include "RenderAPI/GpuParams.h"
#include "RenderAPI/RenderAPI.h"
#include "RenderAPI/GpuParamBlockBuffer.h"
#include "ParamBlocks.g.h"

template<class T>
class FParamBlockParam {
public:
    FParamBlockParam() = default;
    FParamBlockParam(const FGpuParamDataDesc &paramDesc) : mParamDesc(paramDesc) {}

    void set(FGpuParamBlockBuffer *paramBlock, const T &value, uint32_t arrayIdx = 0) const {
#if DEBUG_MODE
        if (arrayIdx >= mParamDesc.arraySize) {
            EXCEPT(FLogRenderer, InvalidParametersException, TEXT("Array index out of range. Array size: %ld. Requested size: %ld"), mParamDesc.arraySize, arrayIdx);
        }
#endif

        uint32_t elementSizeBytes = mParamDesc.elementSize * sizeof(uint32_t);
        uint32_t sizeBytes = std::min(elementSizeBytes, (uint32_t)sizeof(T)); // Truncate if it doesn't fit within parameter size


        paramBlock->write((mParamDesc.cpuMemOffset + arrayIdx * mParamDesc.arrayElementStride) * sizeof(uint32_t),
                          &value, sizeBytes);

        // Set unused bytes to 0
        if (sizeBytes < elementSizeBytes) {
            uint32_t diffSize = elementSizeBytes - sizeBytes;
            paramBlock->zeroOut((mParamDesc.cpuMemOffset + arrayIdx * mParamDesc.arrayElementStride) * sizeof(uint32_t) +
                                sizeBytes, diffSize);
        }
    }

    T get(FGpuParamBlockBuffer *paramBlock, uint32_t arrayIdx = 0) const {
#if DEBUG_MODE
        if (arrayIdx >= mParamDesc.arraySize) {
            EXCEPT(FLogRenderer, InvalidParametersException, TEXT("Array index out of range. Array size: %ld. Requested size: %ld"), mParamDesc.arraySize, arrayIdx);
            return T();
        }
#endif

        uint32_t elementSizeBytes = mParamDesc.elementSize * sizeof(uint32_t);
        uint32_t sizeBytes = std::min(elementSizeBytes, (uint32_t)sizeof(T));

        T value;
        paramBlock->read((mParamDesc.cpuMemOffset + arrayIdx * mParamDesc.arrayElementStride) * sizeof(uint32_t), &value,
                         sizeBytes);

        return value;
    }

protected:
    FGpuParamDataDesc mParamDesc;
};

struct DLL_EXPORT FParamBlock {
    virtual ~FParamBlock() = default;
    virtual void initialize() = 0;
};

QCLASS()
class DLL_EXPORT FParamBlockManager : public TModule<FParamBlockManager> {
    GENERATED_BODY();

private:
    static TArray<FParamBlock *> ToInitialize;

public:
    FParamBlockManager();

public:
    static void RegisterBlock(FParamBlock *paramBlock);
    static void UnregisterBlock(FParamBlock *paramBlock);
};

/**
 * Starts a new custom parameter block. Custom parameter blocks allow you to create C++ structures that map directly
 * to GPU program buffers (for example uniform buffer in OpenGL or constant buffer in DX). Must be followed by
 * BS_PARAM_BLOCK_END.
 */
#define PARAM_BLOCK_BEGIN(Name)																							    \
    struct Name	: FParamBlock {																								\
        Name()																												\
        {																													\
            FParamBlockManager::RegisterBlock(this);                                                                        \
        }																													\
                                                                                                                            \
        FGpuParamBlockBuffer* createBuffer() const { return FGpuParamBlockBuffer::New(mBlockSize); }					    \
                                                                                                                            \
    private:																												\
        friend class FParamBlockManager;																					\
                                                                                                                            \
        void initialize() override {																						\
            mParams = getEntries();																							\
            auto& engine = FRenderAPI::Instance();																		    \
                                                                                                                            \
            FGpuParamBlockDesc blockDesc = engine.generateParamBlockDesc(TEXT(#Name), mParams);                             \
            mBlockSize = blockDesc.blockSize * sizeof(uint32_t);															\
                                                                                                                            \
            initEntries();																									\
        }																													\
                                                                                                                            \
        struct META_FirstEntry {};																							\
        static void META_GetPrevEntries(TArray<FGpuParamDataDesc>& params, META_FirstEntry id) { }							\
        void META_InitPrevEntry(const TArray<FGpuParamDataDesc>& params, uint32_t idx, META_FirstEntry id) { }				\
                                                                                                                            \
        typedef META_FirstEntry

    /**
     * Registers a new entry in a parameter block. Must be called in between BS_PARAM_BLOCK_BEGIN and BS_PARAM_BLOCK_END calls.
     */
#define PARAM_BLOCK_ENTRY_ARRAY(Type, Name, NumElements)																	\
        META_Entry_##Name;																									\
                                                                                                                            \
        struct META_NextEntry_##Name {};																					\
        static void META_GetPrevEntries(TArray<FGpuParamDataDesc>& params, META_NextEntry_##Name id)						\
        {																													\
            META_GetPrevEntries(params, META_Entry_##Name());																\
                                                                                                                            \
            params.add(FGpuParamDataDesc());																			    \
            FGpuParamDataDesc& newEntry = params.top();																		\
            newEntry.name = TEXT(#Name);																					\
            newEntry.type = (EGpuParamDataType)TGpuDataParamInfo<Type>::TypeId;												\
            newEntry.arraySize = NumElements;																				\
            newEntry.elementSize = sizeof(Type);																			\
        }																													\
                                                                                                                            \
        void META_InitPrevEntry(const TArray<FGpuParamDataDesc>& params, uint32_t idx, META_NextEntry_##Name id)			\
        {																													\
            META_InitPrevEntry(params, idx - 1, META_Entry_##Name());														\
            Name = FParamBlockParam<Type>(params[idx]);																		\
        }																													\
                                                                                                                            \
    public:																													\
        FParamBlockParam<Type> Name;																						\
                                                                                                                            \
    private:																												\
        typedef META_NextEntry_##Name

    /**
     * Registers a new entry in a parameter block. Must be called in between BS_PARAM_BLOCK_BEGIN and BS_PARAM_BLOCK_END calls.
     */
#define PARAM_BLOCK_ENTRY(Type, Name) PARAM_BLOCK_ENTRY_ARRAY(Type, Name, 1)

    /** Ends parameter block definition. See BS_PARAM_BLOCK_BEGIN. */
#define PARAM_BLOCK_END																									    \
        META_LastEntry;																										\
                                                                                                                            \
        static TArray<FGpuParamDataDesc> getEntries()																		\
        {																													\
            TArray<FGpuParamDataDesc> entries;																				\
            META_GetPrevEntries(entries, META_LastEntry());																	\
            return entries;																									\
        }																													\
                                                                                                                            \
        void initEntries()																									\
        {																													\
            META_InitPrevEntry(mParams, (uint32_t)mParams.length() - 1, META_LastEntry());									\
        }																													\
                                                                                                                            \
        TArray<FGpuParamDataDesc> mParams;																					\
        uint32_t mBlockSize;																								\
    };
