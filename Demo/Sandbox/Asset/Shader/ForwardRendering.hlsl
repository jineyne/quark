#include "CommonInclude.hlsl"

//----------------------------------------------------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------------------------------------------------
struct VertexInputType {
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct LightingResult {
    float4 diffuse;
    float4 specular;
};

//----------------------------------------------------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------------------------------------------------
PixelInputType VSMain(VertexInputType input) {
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    // output.position = mul(input.position, gMatWorld); // gMatWorldViewProj
    // output.position = mul(output.position, gMatWorldViewProj); // gMatWorld
    output.position = mul(input.position, gMatWorldViewProj);

    output.normal = mul(input.normal, (float3x3) gMatWorld);
    output.normal = normalize(output.normal);

    // Store the input color for the pixel shader to use.
    output.texCoord = input.texCoord;

    return output;
}

//----------------------------------------------------------------------------------------------------------------------
// Pixel Shader Functions
//----------------------------------------------------------------------------------------------------------------------
LightingResult DoPointLight(Light light, Material mat, float4 viewVector, float4 position, float4 normal) {
    LightingResult result;
 
    float4 L = float4(light.Position, 1.0f) - position;
    float distance = length( L );
    L = L / distance;
 
    float attenuation = DoAttenuation(light, distance);
 
    result.diffuse = DoDiffuse( light, L, normal) *  attenuation * light.Intensity;
    result.specular = DoSpecular( light, mat, viewVector, L, normal) * attenuation * light.Intensity;
 
    return result;
}

LightingResult DoDirectionalLight(Light light, Material mat, float4 viewVector, float4 position, float4 normal) {
    LightingResult result;

    float4 L = normalize(-float4(light.Direction, 0.0f));

    result.diffuse = DoDiffuse(light, L, normal) * light.Intensity;
    result.specular = DoSpecular(light, mat, viewVector, L, normal) * light.Intensity;

    return result;
}

LightingResult DoLighting(float4 p, float4 n) {
    float4 eyePos = { 0, 0, 0, 1 };
    float4 v = normalize(eyePos - p);

    LightingResult result = (LightingResult) 0;

    for (int i = 0; i < gLightCount; ++i) {
        LightingResult temp = (LightingResult) 0;

        if (!gLights[i].Enabled) {
            continue;
        }

        if (gLights[i].Type != DIRECTIONAL_LIGHT && length(float4(gLights[i].Position, 1.0f) - p) > gLights[i].Range) {
            continue;
        }

        switch (gLights[i].Type) {
        case POINT_LIGHT:
            temp = DoPointLight(gLights[i], Mat, v, p, n);
            break;

        case DIRECTIONAL_LIGHT:
            temp = DoDirectionalLight(gLights[i], Mat, v, p, n);
            break;
        }

        result.diffuse += temp.diffuse;
        result.specular += temp.specular;
    }

    return result;
}

[earlydepthstencil]
float4 PSMain(PixelInputType input) : SV_TARGET {
    float4 diffuse = Mat.DiffuseColor;

    if (Mat.HasDiffuseTexture) {
        float4 diffuseTex = DiffuseTexture.Sample(LinearRepeatSampler, input.texCoord);
        if (any(diffuse.rgb)) {
            diffuse *= diffuseTex;
        } else {
            diffuse = diffuseTex;
        }
    }

    float alpha = diffuse.a;
    if (Mat.HasOpacityTexture) {
        alpha = OpacityTexture.Sample(LinearRepeatSampler, input.texCoord);
    }

    LightingResult light = DoLighting(input.position, normalize(float4(input.normal, 0.0)));
    diffuse *= float4(light.diffuse.rgb, 1);

    return float4(diffuse.rgb, alpha * Mat.Opacity);
}