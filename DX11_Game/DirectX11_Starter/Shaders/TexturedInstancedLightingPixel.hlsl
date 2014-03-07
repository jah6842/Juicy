
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
	float3 normal	: NORMAL;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

// Entry point for this pixel shader
float4 main(PS_INPUT input) : SV_TARGET
{
	float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = ObjTexture.Sample( ObjSamplerState, input.texCoord );

	// Set the default output color to the ambient light value for all pixels.
	color = ambientColor;

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

	// Check if the N dot L is greater than zero. 
	// If it is then add the diffuse color to the ambient color. 
	// If not then you need to be careful to not add the diffuse color.
	// The reason being is that the diffuse color could be negative and it
	// will subtract away some of the ambient color in the addition which is not correct.
	if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
    }

	// Saturate the final light color.
    color = saturate(color);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;

	return color;
}