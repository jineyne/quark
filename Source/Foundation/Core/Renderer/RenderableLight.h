#pragma once
#include "CorePrerequisites.h"

#include "Image/Color.h"
#include "Scene/Transform.h"
#include "LightBase.h"
#include "ParamBlocks.h"

static constexpr uint32_t STANDARD_FORWARD_MAX_NUM_LIGHTS = 8;

struct LightData {
    Vector3 position;
    Vector3 direction;
    Color color;
    float spotlightAngle;
    float range;
    float intensity;
    BOOL enabled = false;
    BOOL selected;
    ELightType type;
};

PARAM_BLOCK_BEGIN(LightParamDef)
    PARAM_BLOCK_ENTRY_ARRAY(LightData, gLights, STANDARD_FORWARD_MAX_NUM_LIGHTS)
PARAM_BLOCK_END

extern LightParamDef gLightParamDef;

class DLL_EXPORT RenderableLight {
private:
    LightBase *mInternal;

public:
    RenderableLight(LightBase *light);

public:
    void getParameters(LightData &data);

    friend class SceneInfo;
};
