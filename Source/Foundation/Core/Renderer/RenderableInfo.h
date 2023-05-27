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
    PARAM_BLOCK_ENTRY(Matrix4, gMatWorld)
PARAM_BLOCK_END

extern PerObjectParamDef gPerObjectParamDef;

PARAM_BLOCK_BEGIN(PerCallParamDef)
    PARAM_BLOCK_ENTRY(Matrix4, gMatWorldViewProj)
PARAM_BLOCK_END

extern PerCallParamDef gPerCallParamDef;

class PerObjectBuffer {
public:
    static void Update(GpuParamBlockBuffer *buffer, const Matrix4 &tfrm, const Matrix4 &tfrmNoScale,
                       const Matrix4 &prevTfrm, uint32_t layer);
};

class RenderableElement final : public RenderElement {
public:
    GpuParamBinding perCameraBindings[static_cast<uint32_t>(EGpuProgramType::Count)];

    void draw() const override;
};

class DLL_EXPORT RenderableInfo {
public:
    Matrix4 worldMatrix;
    Matrix4 prevWorldMatrix;
    Renderable *renderable;

    TArray<RenderableElement> elements;

    EPrevFrameDirtyState prevFrameDirtyState = EPrevFrameDirtyState::Clean;
    GpuParamBlockBuffer *perObjectParamBuffer;
    GpuParamBlockBuffer *perCallParamBuffer;

    bool bIsReady;

public:
    RenderableInfo();
    ~RenderableInfo();

public:
    void updatePerObjectBuffer();
    void updatePerCallBuffer(const Matrix4 &view, bool flush = true);

    bool isReady() const { return bIsReady; }
    void setReady(bool ready) { bIsReady = ready; }
};