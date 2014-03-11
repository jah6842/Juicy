
#include "globals.inc"

struct VS_INPUT
{
	float4 position		: POSITION;
	float2 texCoord		: TEXCOORD0;
	float3 normal		: NORMAL;
	float4 color		: COLOR;
};

Texture2D ObjTexture : register(t0);
SamplerState ObjSamplerState : register(s0);

struct VS_OUTPUT
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color	: COLOR;
};

// The entry point for our vertex shader
VS_OUTPUT main( VS_INPUT input )
{
	// Set up output
	VS_OUTPUT output;

	input.position.w = 1.0f;

	output.pos = mul(input.position, world);

	// Texture UVs
	output.texCoord = input.texCoord;

	output.color = input.color;

	return output;
}