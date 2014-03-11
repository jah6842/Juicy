
#include "globals.inc"

// Defines what kind of data to expect as input
// - This should match our input layout!
struct VS_INPUT
{
	float3 position		: POSITION;
	float2 TexCoord		: TEXCOORD0;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

// Defines the output data of our vertex shader
// - At a minimum, you'll need an SV_POSITION
// - Should match the pixel shader's input
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};

// The entry point for our vertex shader
VS_OUTPUT main( VS_INPUT input )
{
	// Set up output
	VS_OUTPUT output;

	// Calculate output position
	matrix worldViewProj = mul(mul(world, view), projection);
	output.Pos = mul(float4(input.position, 1.0f), worldViewProj);

	// Pass on the UVs
	output.TexCoord = input.TexCoord;

	return output;
}