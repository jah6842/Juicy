
#include "globals.inc"

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

// Entry point for this pixel shader
float4 main(PS_INPUT input) : SV_TARGET
{
	return ObjTexture.Sample( ObjSamplerState, input.TexCoord );
}