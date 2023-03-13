#pragma once

#include "CorePrerequisites.h"
#include "VertexDeclaration.h"

class DLL_EXPORT FVertexDataDesc {
private:
    TArray<FVertexElement> mElementList;

public:
    static FVertexDataDesc *New();

public:
    void addElement(EVertexElementType type, EVertexElementSemantic semantic, uint32_t semanticIdx = 0,
                    uint32_t instanceStepRate = 0);

    bool hasElement(EVertexElementSemantic semantic, uint32_t semanticIdx) const;
    uint32_t getElementSize(EVertexElementSemantic semantic) const;
    uint32_t getElementOffsetFromStream(EVertexElementSemantic semantic, uint32_t semanticIdx = 0) const;
    uint32_t getVertexStride() const;

    const FVertexElement &getElement(uint32_t idx) const { return mElementList[idx]; }
    const FVertexElement *getElement(EVertexElementSemantic semantic) const;
    const uint32_t getElementCount() const;

    TArray<FVertexElement> createElements() const;
};
