////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.vs.hlsl
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
cbuffer PerObject : register(b0) {
    matrix gMatWorld;
};

cbuffer PerCall : register(b1) {
    matrix gMatWorldViewProj;
};

//////////////
// TYPEDEFS //
//////////////
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

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input) {
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