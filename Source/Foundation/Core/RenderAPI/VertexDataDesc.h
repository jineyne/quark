#pragma once

#include "CorePrerequisites.h"
#include "VertexDeclaration.h"

class DLL_EXPORT VertexDataDesc {
private:
    TArray<VertexElement> mElementList;

public:
    static VertexDataDesc *New();

public:
    void addElement(EVertexElementType type, EVertexElementSemantic semantic, uint32_t semanticIdx = 0,
                    uint32_t instanceStepRate = 0);

    bool hasElement(EVertexElementSemantic semantic, uint32_t semanticIdx) const;
    uint32_t getElementSize(EVertexElementSemantic semantic) const;
    uint32_t getElementOffsetFromStream(EVertexElementSemantic semantic, uint32_t semanticIdx = 0) const;
    uint32_t getVertexStride() const;

    const VertexElement &getElement(uint32_t idx) const { return mElementList[idx]; }
    const VertexElement *getElement(EVertexElementSemantic semantic) const;
    const uint32_t getElementCount() const;

    TArray<VertexElement> createElements() const;
};
