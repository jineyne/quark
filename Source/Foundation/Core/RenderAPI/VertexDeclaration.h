#pragma once

#include "CorePrerequisites.h"
#include "VertexData.h"

enum class EVertexElementSemantic {
    Position = 0,
    Normal,
    Color,
    TexCoord,
};

enum class EVertexElementType {
    Float1 = 0,
    Float2,
    Float3,
    Float4,
    Color,
    Int1,
    Int2,
    Int3,
    Int4,
    Uint1,
    Uint2,
    Uint3,
    Uint4,
    Count,
    Unknown = 0xffff,
};

class DLL_EXPORT VertexElement {
private:
    EVertexElementType mType;
    EVertexElementSemantic mSemantic;

    uint32_t mSource;
    uint32_t mOffset;
    uint32_t mIndex;
    uint32_t mInstanceStepRate;

public:
    VertexElement() = default;
    VertexElement(uint16_t source, uint32_t offset, EVertexElementType type, EVertexElementSemantic semantic,
                  uint32_t index = 0, uint32_t instanceStepRate = 0);

public:
    static uint32_t GetTypeSize(EVertexElementType type);
    static uint32_t GetTypeCount(EVertexElementType type);
    static size_t GetHash(const VertexElement &element);

public:
    bool operator==(const VertexElement &rhs) const;

    bool operator!=(const VertexElement &rhs) const;

public:
    uint32_t getSize() const;
    EVertexElementType getType() const;
    EVertexElementSemantic getSemantic() const;
    uint32_t getSource() const;
    uint32_t getOffset() const;
    uint32_t getSemanticIndex() const;
    uint32_t getInstanceStepRate() const;
};

class DLL_EXPORT VertexDeclaration {
private:
    uint32_t mId;
    TArray<VertexElement> mElementList;

public:
    explicit VertexDeclaration(const TArray <VertexElement> &elements);
    virtual ~VertexDeclaration() = default;

public:
    static VertexDeclaration *New(VertexDataDesc *desc);

public:
    bool isCompatible(const VertexDeclaration *shaderDeclare);
    uint32_t getId() const { return mId; }
    TArray<VertexElement> getMissingElements(VertexDeclaration *shaderDecl);


    uint32_t getElementCount() const { return static_cast<uint32_t>(mElementList.length()); }
    const VertexElement *getElement(uint32_t index) const;
    const VertexElement *findElementBySemantic(EVertexElementSemantic semantic) const;
    uint32_t getVertexSize() const;
    TArray<VertexElement> getElements() const { return mElementList; }
};
