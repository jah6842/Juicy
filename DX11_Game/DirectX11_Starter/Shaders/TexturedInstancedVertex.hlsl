
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
	float3 Normal	: NORMAL;
};

// The entry point for our vertex shader
VS_OUTPUT main( VS_INPUT input )
{
	// Set up output
	VS_OUTPUT output;

	input.position.w = 1.0f;

	// Calculate output position
	// vertex position vector * instance matrix * view/projection matrix
	output.Pos = mul(mul(input.position, input.instancePosition), viewProj);

	// Texture UVs
	output.TexCoord = input.TexCoord;

	// Vertex normals
	output.Normal = input.Normal;

	return output;
}