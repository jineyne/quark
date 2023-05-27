#pragma once
#include "CorePrerequisites.h"

#include "Exception/Exception.h"
#include "RenderAPI/GpuParamDesc.h"
#include "RenderAPI/GpuParams.h"
#include "RenderAPI/RenderAPI.h"
#include "RenderAPI/GpuParamBlockBuffer.h"
#include "ParamBlocks.g.h"

template<class T>
class TParamBlockParam {
public:
    TParamBlockParam() = default;
    TParamBlockParam(const GpuParamDataDesc &paramDesc) : mParamDesc(paramDesc) {}

    void set(GpuParamBlockBuffer *paramBlock, const T &value, uint32_t arrayIdx = 0) const {
#if DEBUG_MODE
        if (arrayIdx >= mParamDesc.arraySize) {
            EXCEPT(LogRenderer, InvalidParametersException, TEXT("Array index out of range. Array size: %ld. Requested size: %ld"), mParamDesc.arraySize, arrayIdx);
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

    T get(GpuParamBlockBuffer *paramBlock, uint32_t arrayIdx = 0) const {
#if DEBUG_MODE
        if (arrayIdx >= mParamDesc.arraySize) {
            EXCEPT(LogRenderer, InvalidParametersException, TEXT("Array index out of range. Array size: %ld. Requested size: %ld"), mParamDesc.arraySize, arrayIdx);
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
    GpuParamDataDesc mParamDesc;
};

struct DLL_EXPORT ParamBlock {
    virtual ~ParamBlock() = default;
    virtual void initialize() = 0;
};

QCLASS()
class DLL_EXPORT ParamBlockManager : public TModule<ParamBlockManager> {
    GENERATED_BODY();

private:
    static TArray<ParamBlock *> ToInitialize;

public:
    ParamBlockManager();

public:
    static void RegisterBlock(ParamBlock *paramBlock);
    static void UnregisterBlock(ParamBlock *paramBlock);
};

#define PARAM_BLOCK_BEGIN(Name)																							    \
    struct Name	: ParamBlock {																								\
        Name()																												\
        {																													\
            ParamBlockManager::RegisterBlock(this);                                                                        \
        }																													\
                                                                                                                            \
        GpuParamBlockBuffer* createBuffer() const { return GpuParamBlockBuffer::New(mBlockSize); }					    \
                                                                                                                            \
    private:																												\
        friend class ParamBlockManager;																					\
                                                                                                                            \
        void initialize() override {																						\
            mParams = getEntries();																							\
            auto& engine = RenderAPI::Instance();																		    \
                                                                                                                            \
            GpuParamBlockDesc blockDesc = engine.generateParamBlockDesc(TEXT(#Name), mParams);                             \
            mBlockSize = blockDesc.blockSize * sizeof(uint32_t);															\
                                                                                                                            \
            initEntries();																									\
        }																													\
                                                                                                                            \
        struct META_FirstEntry {};																							\
        static void META_GetPrevEntries(TArray<GpuParamDataDesc>& params, META_FirstEntry id) { }							\
        void META_InitPrevEntry(const TArray<GpuParamDataDesc>& params, uint32_t idx, META_FirstEntry id) { }				\
                                                                                                                            \
        typedef META_FirstEntry

#define PARAM_BLOCK_ENTRY_ARRAY(Type, Name, NumElements)																	\
        META_Entry_##Name;																									\
                                                                                                                            \
        struct META_NextEntry_##Name {};																					\
        static void META_GetPrevEntries(TArray<GpuParamDataDesc>& params, META_NextEntry_##Name id)						\
        {																													\
            META_GetPrevEntries(params, META_Entry_##Name());																\
                                                                                                                            \
            params.add(GpuParamDataDesc());																			    \
            GpuParamDataDesc& newEntry = params.top();																		\
            newEntry.name = TEXT(#Name);																					\
            newEntry.type = (EGpuParamDataType)TGpuDataParamInfo<Type>::TypeId;												\
            newEntry.arraySize = NumElements;																				\
            newEntry.elementSize = sizeof(Type);																			\
        }																													\
                                                                                                                            \
        void META_InitPrevEntry(const TArray<GpuParamDataDesc>& params, uint32_t idx, META_NextEntry_##Name id)			\
        {																													\
            META_InitPrevEntry(params, idx - 1, META_Entry_##Name());														\
            Name = TParamBlockParam<Type>(params[idx]);																		\
        }																													\
                                                                                                                            \
    public:																													\
        TParamBlockParam<Type> Name;																						\
                                                                                                                            \
    private:																												\
        typedef META_NextEntry_##Name

#define PARAM_BLOCK_ENTRY(Type, Name) PARAM_BLOCK_ENTRY_ARRAY(Type, Name, 1)

#define PARAM_BLOCK_END																									    \
        META_LastEntry;																										\
                                                                                                                            \
        static TArray<GpuParamDataDesc> getEntries()																		\
        {																													\
            TArray<GpuParamDataDesc> entries;																				\
            META_GetPrevEntries(entries, META_LastEntry());																	\
            return entries;																									\
        }																													\
                                                                                                                            \
        void initEntries()																									\
        {																													\
            META_InitPrevEntry(mParams, (uint32_t)mParams.length() - 1, META_LastEntry());									\
        }																													\
                                                                                                                            \
        TArray<GpuParamDataDesc> mParams;																					\
        uint32_t mBlockSize;																								\
    };
