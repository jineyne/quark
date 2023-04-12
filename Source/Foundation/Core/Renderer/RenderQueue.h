#pragma once

#include "CorePrerequisites.h"
#include "RenderElement.h"

enum class EStateReduction {
    None,
    Material,
    Distance,
};

struct FRenderQueueElement {
    const FRenderElement *element = nullptr;
    uint32_t passIdx = 0;
    uint32_t techniqueIdx = 0;
    bool applyPass = true;
};

class DLL_EXPORT FRenderQueue {
private:
    struct SortableElement {
        uint32_t seqIdx;
        int32_t priority;
        float distFromCamera;
        uint32_t shaderId;
        uint32_t techniqueIdx;
        uint32_t passIdx;
    };

protected:
    TArray<SortableElement> mSortableElementList;
    TArray<uint32_t> mSortableElementIdx;
    TArray<const FRenderElement *> mElementList;

    TArray<FRenderQueueElement> mSortedRenderElementList;
    EStateReduction mStateReductionMode;

public:
    FRenderQueue(EStateReduction mode = EStateReduction::Distance);
    virtual ~FRenderQueue() = default;

public:
    static bool ElementSorterNoGroup(uint32_t aIdx, uint32_t bIdx, const TArray<SortableElement> &lookup);
    static bool ElementSorterPreferGroup(uint32_t aIdx, uint32_t bIdx, const TArray<SortableElement> &lookup);
    static bool ElementSorterPreferDistance(uint32_t aIdx, uint32_t bIdx, const TArray<SortableElement> &lookup);

public:
    void add(const FRenderElement *element, float distFromCamera, uint32_t techniqueIdx);

    void clear();

    virtual void sort();

    const TArray<FRenderQueueElement> &getSortedElementList() const;

    void setStateReduction(EStateReduction mode) { mStateReductionMode = mode; }


};