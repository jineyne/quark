#pragma once

#include "CorePrerequisites.h"
#include "RenderElement.h"

enum class EStateReduction {
    None,
    Material,
    Distance,
};

struct RenderQueueElement {
    const RenderElement *element = nullptr;
    uint32_t passIdx = 0;
    uint32_t techniqueIdx = 0;
    bool applyPass = true;
};

class DLL_EXPORT RenderQueue {
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
    TArray<const RenderElement *> mElementList;

    TArray<RenderQueueElement> mSortedRenderElementList;
    EStateReduction mStateReductionMode;

public:
    RenderQueue(EStateReduction mode = EStateReduction::Distance);
    virtual ~RenderQueue() = default;

public:
    static bool ElementSorterNoGroup(uint32_t aIdx, uint32_t bIdx, const TArray<SortableElement> &lookup);
    static bool ElementSorterPreferGroup(uint32_t aIdx, uint32_t bIdx, const TArray<SortableElement> &lookup);
    static bool ElementSorterPreferDistance(uint32_t aIdx, uint32_t bIdx, const TArray<SortableElement> &lookup);

public:
    void add(const RenderElement *element, float distFromCamera, uint32_t techniqueIdx);

    void clear();

    virtual void sort();

    const TArray<RenderQueueElement> &getSortedElementList() const;

    void setStateReduction(EStateReduction mode) { mStateReductionMode = mode; }


};