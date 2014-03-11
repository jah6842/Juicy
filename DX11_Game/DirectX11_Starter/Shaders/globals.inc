// Include me in any shaders that need to access constant buffers
// I.E. probably all of them :)

// The constant buffer that holds our "per frame" data
cbuffer perFrame : register( b0 )
{
	matrix view;		// Camera view matrix
	matrix projection;	// Camera projection matrix
	matrix viewProj;	// Camera view*proj matrix
};

// The constant buffer that holds our "per model" data
cbuffer perModel : register( b1 )
{
	matrix world;
};

// Constant buffer for directional light
cbuffer DirectionalLight : register(b2)
{
	float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};