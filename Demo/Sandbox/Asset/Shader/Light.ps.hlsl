////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.ps.hlsl
////////////////////////////////////////////////////////////////////////////////
Texture2D gTexture;
SamplerState gSamplerState;

cbuffer LightParam {
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET {
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 textureColor = gTexture.Sample(gSamplerState, input.texCoord);

    // Invert the light direction for calculations.
    float3 lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    float lightIntensity = saturate(dot(input.normal, lightDir));

     // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
    float4 color = color = ambientColor;

    if (lightIntensity > 0.0f) {
        color += saturate(diffuseColor * lightIntensity);
    }

    color = saturate(color);

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    return color * textureColor;
}