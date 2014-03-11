
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
};

// The entry point for our vertex shader
VS_OUTPUT main( VS_INPUT input )
{
	// Set up output
	VS_OUTPUT output;

	// Make our position float4
	input.position.w = 1.0f;

	// Calculate output position
	// vertex position vector * instance matrix * view/projection matrix
	output.position = mul(mul(input.position, input.instancePosition), viewProj);

	// Texture UVs
	output.texCoord = input.texCoord;

	// Calculate the normal vector against the world matrix
	output.normal = mul(input.normal, (float3x3)input.instancePosition);

	// Normalize
	output.normal = normalize(output.normal);

	return output;
}