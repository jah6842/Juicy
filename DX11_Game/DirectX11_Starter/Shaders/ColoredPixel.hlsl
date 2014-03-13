
#include "globals.inc"

struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};

// Entry point for this pixel shader
float4 main(PS_INPUT input) : SV_TARGET
{
	return float4(1,0,0,1);
	//return input.color;
}