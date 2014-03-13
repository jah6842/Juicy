
#include "globals.inc"

struct VS_INPUT
{
	float4 position		: POSITION;
	float2 texCoord		: TEXCOORD0;
	float3 normal		: NORMAL;
	float4 color		: COLOR;
	matrix instancePosition : INSTANCEPOS;
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};

// The entry point for our vertex shader
VS_OUTPUT main( VS_INPUT input )
{
	// Set up output
	VS_OUTPUT output;

	input.position.w = 1.0f;

	// Calculate output position
	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(input.position, worldViewProj);

	// Pass the color through - will be interpolated per-pixel by the rasterizer
	output.color = input.color;

	return output;
}