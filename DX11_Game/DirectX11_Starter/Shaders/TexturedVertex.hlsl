
#include "globals.inc"

// Defines what kind of data to expect as input
// - This should match our input layout!
struct VS_INPUT
{
	float4 position		: POSITION;
	float2 TexCoord		: TEXCOORD0;
	float3 Normal		: NORMAL;
	float4 Color		: COLOR;
	matrix instancePosition : INSTANCEPOS;
};

Texture2D ObjTexture : register(t0);
SamplerState ObjSamplerState : register(s0);

// Defines the output data of our vertex shader
// - At a minimum, you'll need an SV_POSITION
// - Should match the pixel shader's input
struct VS_OUTPUT
{
	float4 Pos		: SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};

// The entry point for our vertex shader
VS_OUTPUT main( VS_INPUT input )
{
	// Set up output
	VS_OUTPUT output;

	// Calculate output position
	matrix worldViewProj = mul(mul(world, view), projection);
	output.Pos = mul(float4(input.position.xyz, 1.0f), worldViewProj);

	// Pass on the UVs and normals
	output.TexCoord = input.TexCoord;

	return output;
}