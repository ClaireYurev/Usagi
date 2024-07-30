/*
The rasterizer determines the pixel positions.
The pixel shader just determines the colors of each of those pixels.
Usually, the pixel shader does not need the coordinate of a pixel in screen space in order to accomplish this.

SV_Target as in: SYSTEM_VALUE_RENDER_TARGET

*/

float4 main() : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}