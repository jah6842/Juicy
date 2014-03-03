
// The constant buffer that holds our "per model" data
// - Each object you draw with this shader will probably have
//   slightly different data (at least for the world matrix)
cbuffer perModel : register( b0 )
{
	matrix viewProj;
};

// Defines what kind of data to expect as input
// - This should match our input layout!
struct VertexShaderInput
{
	float3 position		: POSITION;
	float2 TexCoord		: TEXCOORD0;
	matrix instancePosition : INSTANCEPOS;
};

Texture2D ObjTexture : register(t0);
SamplerState ObjSamplerState : register(s0);

// Defines the output data of our vertex shader
// - At a minimum, you'll need an SV_POSITION
// - Should match the pixel shader's input
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};

// The entry point for our vertex shader
VS_OUTPUT main( VertexShaderInput input )
{
	// Set up output
	VS_OUTPUT output;

	// Calculate output position
	// vertex position vector * instance matrix * view/projection matrix
	output.Pos = mul(mul(float4(input.position, 1.0f), input.instancePosition), viewProj);

	// Texture UVs
	output.TexCoord = input.TexCoord;

	return output;
}