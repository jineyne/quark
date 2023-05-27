#include "RenderableLight.h"

LightParamDef gLightParamDef;

RenderableLight::RenderableLight(LightBase *light) : mInternal(light) { }

void RenderableLight::getParameters(LightData &data) {
    data.position = mInternal->getTransform()->getPosition();
    data.direction = mInternal->getTransform()->getRotation() * Vector3(0, 0, 1);

    data.color = mInternal->getColor();
    data.spotlightAngle = mInternal->getSpotAngle();
    data.range = mInternal->getRange();
    data.intensity = mInternal->getIntensity();
    data.type = mInternal->getType();
    data.enabled = true; // mTable->getActive();
}
