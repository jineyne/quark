////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
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
    // float4 color : COLOR;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float4 color : COLOR;
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

    // Store the input color for the pixel shader to use.
    output.color = float4(1.0f, 0.0f, 0.0f, 1.0f);

    return output;
}