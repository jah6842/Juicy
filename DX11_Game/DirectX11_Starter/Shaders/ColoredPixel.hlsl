
#include "globals.inc"

struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};

// Entry point for this pixel shader
float4 main(PS_INPUT input) : SV_TARGET
{
	return input.color;
}