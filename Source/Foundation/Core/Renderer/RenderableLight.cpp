#include "RenderableLight.h"

LightParamDef gLightParamDef;

FRenderableLight::FRenderableLight(FLightBase *light) : mInternal(light) { }

void FRenderableLight::getParameters(FLightData &data) {
    data.position = mInternal->getTransform()->getPosition();
    data.direction = mInternal->getTransform()->getRotation() * FVector3(0, 0, 1);

    data.color = mInternal->getColor();
    data.spotlightAngle = mInternal->getSpotAngle();
    data.range = mInternal->getRange();
    data.intensity = mInternal->getIntensity();
    data.type = mInternal->getType();
    data.enabled = true; // mInternal->getActive();
}
