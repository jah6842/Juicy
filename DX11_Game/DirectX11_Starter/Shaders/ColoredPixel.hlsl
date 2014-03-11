
#include "globals.inc"

struct PS_INPUT
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

// Entry point for this pixel shader
float4 main(PS_INPUT input) : SV_TARGET
{
	return input.color;
}