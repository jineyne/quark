//----------------------------------------------------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------------------------------------------------
cbuffer PerObject : register(b0) {
    matrix gMatWorld;
};

cbuffer PerCall : register(b1) {
    matrix gMatWorldViewProj;
};

Texture2D DiffuseTexture         : register( t0 );
SamplerState LinearRepeatSampler : register( s0 );

struct VertexInputType {
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
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

    // Store the input color for the pixel shader to use.
    output.texCoord = input.texCoord;

    return output;
}

[earlydepthstencil]
float4 PSMain(PixelInputType input) : SV_TARGET {
    return DiffuseTexture.Sample(LinearRepeatSampler, input.texCoord);
}