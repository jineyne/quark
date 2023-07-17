//----------------------------------------------------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------------------------------------------------
struct Material {
    float4  GlobalAmbient;
    //-------------------------- ( 16 bytes )
    float4  AmbientColor;
    //-------------------------- ( 16 bytes )
    float4  EmissiveColor;
    //-------------------------- ( 16 bytes )
    float4  DiffuseColor;
    //-------------------------- ( 16 bytes )
    float4  SpecularColor;
    //-------------------------- ( 16 bytes )
    // Reflective value.
    float4  Reflectance;
    //-------------------------- ( 16 bytes )
    float   Opacity;
    float   SpecularPower;
    // For transparent materials, IOR > 0.
    float   IndexOfRefraction;
    bool    HasAmbientTexture;
    //-------------------------- ( 16 bytes )
    bool    HasEmissiveTexture;
    bool    HasDiffuseTexture;
    bool    HasSpecularTexture;
    bool    HasSpecularPowerTexture;
    //-------------------------- ( 16 bytes )
    bool    HasNormalTexture;
    bool    HasBumpTexture;
    bool    HasOpacityTexture;
    float   BumpIntensity;
    //-------------------------- ( 16 bytes )
    float   SpecularScale;
    float   AlphaThreshold;
    float2  Padding;
    //--------------------------- ( 16 bytes )
};  //--------------------------- ( 16 * 10 = 160 bytes )

Texture2D AmbientTexture         : register( t0 );
Texture2D EmissiveTexture        : register( t1 );
Texture2D DiffuseTexture         : register( t2 );
Texture2D SpecularTexture        : register( t3 );
Texture2D SpecularPowerTexture   : register( t4 );
Texture2D NormalTexture          : register( t5 );
Texture2D BumpTexture            : register( t6 );
Texture2D OpacityTexture         : register( t7 );

SamplerState LinearRepeatSampler : register( s0 );
SamplerState LinearClampSampler  : register( s1 );

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2

struct Light {
    /**
     * Position for point and spot lights (World space).
     */
    float3   Position;
    //--------------------------------------------------------------( 12 bytes )
    /**
     * Direction for spot and directional lights (World space).
     */
    float3   Direction;
    //--------------------------------------------------------------( 12 bytes )
    /**
     * Color of the light. Diffuse and specular colors are not seperated.
     */
    float4   Color;
    //--------------------------------------------------------------( 16 bytes )
    /**
     * The half angle of the spotlight cone.
     */
    float    SpotlightAngle;
    /**
     * The range of the light.
     */
    float    Range;

    /**
     * The intensity of the light.
     */
    float    Intensity;

    /**
     * Disable or enable the light.
     */
    bool    Enabled;
    //--------------------------------------------------------------( 16 bytes )

    /**
     * Is the light selected in the editor?
     */
    bool    Selected;

    /**
     * The type of the light.
     */
    uint    Type;
    //--------------------------------------------------------------( 8 bytes )
    //--------------------------------------------------------------( 12 * 2 + 8 + 16 * 2 = 64 bytes )
};

//----------------------------------------------------------------------------------------------------------------------
// Globals
//----------------------------------------------------------------------------------------------------------------------
cbuffer PerObject : register(b0) {
    matrix gMatWorld;
};

cbuffer PerCall : register(b1) {
    matrix gMatWorldViewProj;
};

cbuffer Material : register(b2) {
    Material Mat;
}

int gLightCount;
StructuredBuffer<Light> gLights;

//----------------------------------------------------------------------------------------------------------------------
// Helper Methods
//----------------------------------------------------------------------------------------------------------------------
float4 DoDiffuse( Light light, float4 lightVector, float4 normal) {
    return light.Color * max(dot(normal, lightVector), 0);
}

float4 DoSpecular(Light light, Material material, float4 viewVector, float4 lightVector, float4 normal) {
    float4 r = normalize(reflect(-lightVector, normal));
    float d = max(dot(r, viewVector), 0);
    float p = pow(d, material.SpecularPower);

    return light.Color * p;
}

float DoAttenuation(Light light, float distance) {
    return 1.0f - smoothstep(light.Range * 0.75f, light.Range, distance);
}

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