
#include "globals.inc"

// Should match VS_OUTPUT
struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color	: COLOR;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

// Entry point for this pixel shader
float4 main(PS_INPUT input) : SV_TARGET
{
	float4 diffuse =  ObjTexture.Sample( ObjSamplerState, input.texCoord );
    clip(diffuse.a - .5);
	//float4 diffuse = float4(input.texCoord.x,input.texCoord.y,0,1); // Debug for showing UV as color
	diffuse *= input.color;
	return diffuse;
}