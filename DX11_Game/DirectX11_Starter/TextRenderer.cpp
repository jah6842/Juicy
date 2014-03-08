#include "TextRenderer.h"

ID3D11ShaderResourceView*	TextRenderer::fontTexture		= nullptr;
ID3D11SamplerState*			TextRenderer::fontTextureSampler= nullptr;
ID3D11Buffer*				TextRenderer::fontVBuffer		= nullptr;
ID3D11VertexShader*			TextRenderer::fontVShader		= nullptr;
ID3D11PixelShader*			TextRenderer::fontPShader		= nullptr;

void TextRenderer::Setup(){
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	// Load Texture
	std::wstring texPath = TEXTURE_PATH;
	texPath += L"ascii.png";
	DirectX::CreateWICTextureFromFile(device, deviceContext, texPath.c_str(), NULL, &fontTexture);

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    
	//Create the Sample State
	device->CreateSamplerState( &sampDesc, &fontTextureSampler );

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	std::wstring vShaderPath = SHADER_PATH;
	vShaderPath += L"FontVertex.cso";
	D3DReadFileToBlob(vShaderPath.c_str(), &vsBlob);

	// Create the shader on the device
	HR(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&fontVShader));

	ReleaseMacro(vsBlob);

	// Load Pixel Shader ---------------------------------------
	ID3DBlob* psBlob;
	std::wstring pShaderPath = SHADER_PATH;
	pShaderPath += L"FontPixel.cso";
	D3DReadFileToBlob(pShaderPath.c_str(), &psBlob);

	// Create the shader on the device
	HR(device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&fontPShader));

	// Clean up
	ReleaseMacro(psBlob);
};

void TextRenderer::Cleanup(){
	ReleaseMacro(fontTexture);
	ReleaseMacro(fontTextureSampler);
	ReleaseMacro(fontPShader);
	ReleaseMacro(fontVShader);
};