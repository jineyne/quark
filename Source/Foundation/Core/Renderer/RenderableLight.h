#pragma once
#include "CorePrerequisites.h"

#include "Image/Color.h"
#include "Scene/Transform.h"
#include "LightBase.h"
#include "ParamBlocks.h"

static constexpr uint32_t STANDARD_FORWARD_MAX_NUM_LIGHTS = 8;

struct FLightData {
    FVector3 position;
    FVector3 direction;
    FColor color;
    float spotlightAngle;
    float range;
    float intensity;
    BOOL enabled = false;
    BOOL selected;
    ELightType type;
};

PARAM_BLOCK_BEGIN(LightParamDef)
    PARAM_BLOCK_ENTRY_ARRAY(FLightData, gLights, STANDARD_FORWARD_MAX_NUM_LIGHTS)
PARAM_BLOCK_END

extern LightParamDef gLightParamDef;

class DLL_EXPORT FRenderableLight {
private:
    FLightBase *mInternal;

public:
    FRenderableLight(FLightBase *light);

public:
    void getParameters(FLightData &data);

    friend class FSceneInfo;
};
