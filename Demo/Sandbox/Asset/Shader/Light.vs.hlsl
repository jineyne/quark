////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.vs.hlsl
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
cbuffer PerObject : register(b1) {
    matrix gWorldMat;
    matrix gViewMat;
    matrix gProjMat;
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
PixelInputType main(VertexInputType input)
{
    PixelInputType output;


    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, gWorldMat);
    output.position = mul(output.position, gViewMat);
    output.position = mul(output.position, gProjMat);

    output.normal = mul(input.normal, (float3x3) gWorldMat);

    output.normal = normalize(output.normal);

    // Store the input color for the pixel shader to use.
    output.texCoord = input.texCoord;

    return output;
}