/* Vertex Shader needs to output a position.

It needs to give out a position of the vertex so that it can be rasterized on the screen.

A position has to be a four-dimensional vector: float4, because of homogeneous coordinates.

Homogeneous coordinates and projective transform matrix.

SHADER INPUTS:
Data type (float2, float3, float4 - usually vectors, though sometimes other things)
Name of the Variable
Semantic: what are you using this thing for?

Inputs and outputs for a shader must be labeled with semantics, generally.

The output LABEL is a SYSTEM_VALUE_Position.

It needs a position of each vertex is order to rasterize it.

*/

struct VSOut
{   
    // Order matters: pos first and color second won't work
    float3 color : Color;
    float4 pos : SV_Position;
};

VSOut main(float2 pos : Position, float3 color : Color)
{
    VSOut vso;
    vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
    vso.color = color;
    return vso;
}

