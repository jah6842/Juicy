
#include "globals.inc"

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 TexCoord : TEXCOORD0;
};

TextureCube ObjTextureCube : register(t0);
SamplerState ObjSamplerState : register(s0);

// Entry point for this pixel shader
float4 main(PS_INPUT input) : SV_TARGET
{
	return ObjTextureCube.Sample( ObjSamplerState, input.TexCoord ) * 5.0f;
}