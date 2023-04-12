#pragma once

#include "CorePrerequisites.h"
#include "ParamBlocks.h"
#include "RenderElement.h"
#include "Renderable.h"

enum class EPrevFrameDirtyState {
    Updated,
    CopyMostRecent,
    Clean
};

PARAM_BLOCK_BEGIN(PerObjectParamDef)
    PARAM_BLOCK_ENTRY(FMatrix4, gMatWorld)
    /*PARAM_BLOCK_ENTRY(FMatrix4, gMatInvWorld)
    PARAM_BLOCK_ENTRY(FMatrix4, gMatWorldNoScale)
    PARAM_BLOCK_ENTRY(FMatrix4, gMatInvWorldNoScale)
    PARAM_BLOCK_ENTRY(FMatrix4, gMatPrevWorld)
    PARAM_BLOCK_ENTRY(float, gWorldDeterminantSign)
    PARAM_BLOCK_ENTRY(int32_t, gLayer)*/
PARAM_BLOCK_END

PARAM_BLOCK_BEGIN(PerCallParamDef)
    PARAM_BLOCK_ENTRY(FMatrix4, gMatWorldViewProj)
PARAM_BLOCK_END

extern PerObjectParamDef gPerObjectParamDef;
extern PerCallParamDef gPerCallParamDef;

class FPerObjectBuffer {
public:
    static void Update(FGpuParamBlockBuffer *buffer, const FMatrix4 &tfrm, const FMatrix4 &tfrmNoScale,
                       const FMatrix4 &prevTfrm, uint32_t layer);
};

class FRenderableElement final : public FRenderElement {
public:
    GpuParamBinding perCameraBindings[static_cast<uint32_t>(EGpuProgramType::Count)];

    void draw() const override;
};

class DLL_EXPORT FRenderableInfo {
public:
    FMatrix4 worldMatrix;
    FMatrix4 prevWorldMatrix;
    FRenderable *renderable;

    TArray<FRenderableElement> elements;

    EPrevFrameDirtyState prevFrameDirtyState = EPrevFrameDirtyState::Clean;
    FGpuParamBlockBuffer *perObjectParamBuffer;
    FGpuParamBlockBuffer *perCallParamBuffer;

    bool bIsReady;

public:
    FRenderableInfo();
    ~FRenderableInfo();

public:
    void updatePerObjectBuffer();
    void updatePerCallBuffer(const FMatrix4 &view, bool flush = true);

    bool isReady() const { return bIsReady; }
    void setReady(bool ready) { bIsReady = ready; }
};