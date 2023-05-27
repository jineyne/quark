#include "RenderQueue.h"

RenderQueue::RenderQueue(EStateReduction mode) : mStateReductionMode(mode) {}

bool RenderQueue::ElementSorterNoGroup(uint32_t aIdx, uint32_t bIdx, const TArray<SortableElement> &lookup) {
    const SortableElement &a = lookup[aIdx];
    const SortableElement &b = lookup[bIdx];

    uint8_t isHigher = (a.priority > b.priority) << 2 | (a.distFromCamera < b.distFromCamera) << 1 |
                       (a.seqIdx < b.seqIdx);

    uint8_t isLower = (a.priority < b.priority) << 2 | (a.distFromCamera > b.distFromCamera) << 1 |
                      (a.seqIdx > b.seqIdx);

    return isHigher > isLower;
}

bool RenderQueue::ElementSorterPreferGroup(uint32_t aIdx, uint32_t bIdx, const TArray<SortableElement> &lookup) {
    const SortableElement &a = lookup[aIdx];
    const SortableElement &b = lookup[bIdx];

    uint8_t isHigher = (a.priority > b.priority) << 5 | (a.shaderId < b.shaderId) << 4 |
                       (a.techniqueIdx < b.techniqueIdx) << 3 | (a.passIdx < b.passIdx) << 2 |
                       (a.distFromCamera < b.distFromCamera) << 1 | (a.seqIdx < b.seqIdx);

    uint8_t isLower = (a.priority < b.priority) << 5 | (a.shaderId > b.shaderId) << 4 |
                      (a.techniqueIdx > b.techniqueIdx) << 3 | (a.passIdx > b.passIdx) << 2 |
                      (a.distFromCamera > b.distFromCamera) << 1 | (a.seqIdx > b.seqIdx);

    return isHigher > isLower;
}

bool RenderQueue::ElementSorterPreferDistance(uint32_t aIdx, uint32_t bIdx, const TArray<SortableElement> &lookup) {
    const SortableElement &a = lookup[aIdx];
    const SortableElement &b = lookup[bIdx];

    uint8_t isHigher = (a.priority > b.priority) << 5 | (a.distFromCamera < b.distFromCamera) << 4 |
                       (a.shaderId < b.shaderId) << 3 | (a.techniqueIdx < b.techniqueIdx) << 2 |
                       (a.passIdx < b.passIdx) << 1 | (a.seqIdx < b.seqIdx);

    uint8_t isLower = (a.priority < b.priority) << 5 | (a.distFromCamera > b.distFromCamera) << 4 |
                      (a.shaderId > b.shaderId) << 3 | (a.techniqueIdx > b.techniqueIdx) << 2 |
                      (a.passIdx > b.passIdx) << 1 | (a.seqIdx > b.seqIdx);

    return isHigher > isLower;
}

void RenderQueue::add(const RenderElement *element, float distFromCamera, uint32_t techniqueIdx) {
    auto material = element->material;
    auto shader = material->getShader();

    uint32_t queuePriority = shader->getQueuePriority();
    EQueueSortType sortType = shader->getQueueSortType();

    uint32_t shaderId = shader->getId();
    bool separablePasses = shader->getAllowSeparablePasses();

    switch (sortType) {
        case EQueueSortType::None:
            distFromCamera = 0;
            break;

        case EQueueSortType::BackToFront:
            distFromCamera = -distFromCamera;
            break;

        case EQueueSortType::FrontToBack:
            break;
    }

    uint32_t passCount = material->getPassesCount(techniqueIdx);
    if (!separablePasses) {
        passCount = std::min(1U, passCount);
    }

    for (uint32_t i = 0; i < passCount; i++) {
        auto idx = static_cast<uint32_t>(mSortableElementIdx.length());
        mSortableElementIdx.add(idx);

        mSortableElementList.add(SortableElement());
        auto &sortableElement = mSortableElementList.top();

        sortableElement.seqIdx = idx;
        sortableElement.priority = queuePriority;
        sortableElement.shaderId = shaderId;
        sortableElement.techniqueIdx = techniqueIdx;
        sortableElement.passIdx = i;
        sortableElement.distFromCamera = distFromCamera;

        mElementList.add(element);
    }
}

void RenderQueue::clear() {
    mSortableElementList.clear();
    mSortableElementIdx.clear();
    mElementList.clear();

    mSortedRenderElementList.clear();
}

void RenderQueue::sort() {
    std::function<bool(uint32_t, uint32_t, const TArray<SortableElement>&)> sortFunc;

    switch (mStateReductionMode) {
        case EStateReduction::None:
            sortFunc = &RenderQueue::ElementSorterNoGroup;
            break;

        case EStateReduction::Material:
            sortFunc = &RenderQueue::ElementSorterPreferGroup;
            break;

        case EStateReduction::Distance:
            sortFunc = &RenderQueue::ElementSorterPreferDistance;
            break;
    }

    std::sort(mSortableElementIdx.begin(), mSortableElementIdx.end(), [&](uint32_t a, uint32_t b) {
        return sortFunc(a, b, mSortableElementList);
    });

    auto prevShaderId = static_cast<uint32_t>(-1);
    auto prevTechniqueIdx = static_cast<uint32_t>(-1);
    auto prevPassIdx = static_cast<uint32_t>(-1);

    for (uint32_t i = 0; i < static_cast<uint32_t>(mSortableElementIdx.length()); i++) {
        const auto idx = mSortableElementIdx[i];
        const auto &elem = mSortableElementList[i];
        const auto renderElem = mElementList[i];

        const bool separablePasses = renderElem->material->getShader()->getAllowSeparablePasses();

        if (separablePasses) {
            mSortedRenderElementList.add(RenderQueueElement());

            auto &sortedElem = mSortedRenderElementList.top();
            sortedElem.element = renderElem;
            sortedElem.techniqueIdx = elem.techniqueIdx;
            sortedElem.passIdx = elem.passIdx;

            if (prevShaderId != elem.shaderId || prevTechniqueIdx != elem.techniqueIdx ||
                prevPassIdx != elem.passIdx) {
                sortedElem.applyPass = true;
                prevShaderId = elem.shaderId;
                prevTechniqueIdx = elem.techniqueIdx;
                prevPassIdx = elem.passIdx;
            } else {
                sortedElem.applyPass = false;
            }
        } else {
            const uint32_t passCount = renderElem->material->getPassesCount(elem.techniqueIdx);
            for (uint32_t j = 0; j < passCount; j++) {
                mSortedRenderElementList.add(RenderQueueElement());

                auto &sortedElem = mSortedRenderElementList.top();
                sortedElem.element = renderElem;
                sortedElem.techniqueIdx = elem.techniqueIdx;
                sortedElem.passIdx = j;

                if (prevShaderId != elem.shaderId || prevTechniqueIdx != elem.techniqueIdx ||
                    prevPassIdx != elem.passIdx) {
                    sortedElem.applyPass = true;
                    prevShaderId = elem.shaderId;
                    prevTechniqueIdx = elem.techniqueIdx;
                    prevPassIdx = j;
                } else {
                    sortedElem.applyPass = false;
                }
            }
        }
    }
}

const TArray<RenderQueueElement> &RenderQueue::getSortedElementList() const {
    return mSortedRenderElementList;
}
