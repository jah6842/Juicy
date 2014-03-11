
// Constant buffer for directional light
cbuffer DirectionalLight : register(b2)
{
	float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

// Should match VS_OUTPUT
struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

// Entry point for this pixel shader
float4 main(PS_INPUT input) : SV_TARGET
{
	float4 diffuse =  ObjTexture.Sample( ObjSamplerState, input.texCoord );
    clip(diffuse.a - 0.25);
	//float4 diffuse = float4(input.texCoord.x,input.texCoord.y,0,1);
	return diffuse;
}