#include "TextureManager.h"

// Make sure this matches with the enum in the header!
const WCHAR* textureNames[TM_NUM_TEXTURES] = { 
	L"marble.png", 
	L"sand.jpg",
	L"scales.png"
};

//Texture TEXTURE_MARBLE = { L"marble.png", TM_TEXTURE_MARBLE, nullptr, true, false, false };
//Texture TEXTURE_SAND = { L"sand.jpg", TM_TEXTURE_SAND, nullptr, true, false, false };
//Texture TEXTURE_SCALES = { L"scales.png", TM_TEXTURE_SCALES, nullptr, true, false, false };

TextureManager::TextureManager(){
	samplerStatePoint = nullptr;
	samplerStateLinear = nullptr;
	samplerStateAnisotropic1X = nullptr;
	samplerStateAnisotropic2X = nullptr;
	samplerStateAnisotropic4X = nullptr;
	samplerStateAnisotropic8X = nullptr;
	samplerStateAnisotropic16X = nullptr;
};

TextureManager::~TextureManager(){

};

void TextureManager::Init(){
	InitSamplerStates();
	InitTextures();
};

void TextureManager::Cleanup(){
	ReleaseSamplerStates();
	ReleaseTextures();
};

void TextureManager::SetActiveTexture(TM_TEXTURE t, TM_TEXTURE_TYPE type){
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();
	deviceContext->PSSetShaderResources(type,1,&textures[t].resourceView);
};

void TextureManager::SetActiveFilterMode(TM_FILTER_MODE m){
	
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();
	
	switch(m){
	case TM_FILTER_POINT:
		deviceContext->PSSetSamplers(0,1,&samplerStatePoint);
		break;
	case TM_FILTER_LINEAR:
		deviceContext->PSSetSamplers(0,1,&samplerStateLinear);
		break;
	case TM_FILTER_ANISO_1X:
		deviceContext->PSSetSamplers(0,1,&samplerStateAnisotropic1X);
		break;
	case TM_FILTER_ANISO_2X:
		deviceContext->PSSetSamplers(0,1,&samplerStateAnisotropic2X);
		break;
	case TM_FILTER_ANISO_4X:
		deviceContext->PSSetSamplers(0,1,&samplerStateAnisotropic4X);
		break;
	case TM_FILTER_ANISO_8X:
		deviceContext->PSSetSamplers(0,1,&samplerStateAnisotropic8X);
		break;
	case TM_FILTER_ANISO_16X:
		deviceContext->PSSetSamplers(0,1,&samplerStateAnisotropic16X);
		break;
	}
};

// Create all of the sampler states that we will need
void TextureManager::InitSamplerStates(){

	// Get graphics device
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0.0f;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the Point Filtering Sample State
	HR(device->CreateSamplerState( &sampDesc, &samplerStatePoint ));

	// Create the Linear Filtering Sample State
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	HR(device->CreateSamplerState( &sampDesc, &samplerStateLinear ));

	// Create the 1X Anisotropic Filtering Sample State
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.MaxAnisotropy = 1;
	HR(device->CreateSamplerState( &sampDesc, &samplerStateAnisotropic1X ));

	// Create the 2X Anisotropic Filtering Sample State
	sampDesc.MaxAnisotropy = 2;
	HR(device->CreateSamplerState( &sampDesc, &samplerStateAnisotropic2X ));
	
	// Create the 4X Anisotropic Filtering Sample State
	sampDesc.MaxAnisotropy = 4;
	HR(device->CreateSamplerState( &sampDesc, &samplerStateAnisotropic4X ));

	// Create the 8X Anisotropic Filtering Sample State
	sampDesc.MaxAnisotropy = 8;
	HR(device->CreateSamplerState( &sampDesc, &samplerStateAnisotropic8X ));

	// Create the 16X Anisotropic Filtering Sample State
	sampDesc.MaxAnisotropy = 16;
	HR(device->CreateSamplerState( &sampDesc, &samplerStateAnisotropic16X ));
};

// Load all of the textures that we're going to need
void TextureManager::InitTextures(){

	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	for(UINT i = 0; i < TM_NUM_TEXTURES; i++){
		// Set up a texture struct
		Texture tempTex;
		tempTex.isDiffuseMap = true;
		tempTex.name = textureNames[i];
		tempTex.textureID = i;

		std::wstring wTexturePath = TEXTURE_PATH;
		wTexturePath += textureNames[i];

		// DirectXTK Texture Loading
		HR(CreateWICTextureFromFile(device, deviceContext, wTexturePath.c_str(), NULL, &tempTex.resourceView));

		// Add the texture to the list
		textures[i] = tempTex;
	}
};

void TextureManager::ReleaseSamplerStates(){
	ReleaseMacro(samplerStatePoint);
	ReleaseMacro(samplerStateLinear);
	ReleaseMacro(samplerStateAnisotropic1X);
	ReleaseMacro(samplerStateAnisotropic2X);
	ReleaseMacro(samplerStateAnisotropic4X);
	ReleaseMacro(samplerStateAnisotropic8X);
	ReleaseMacro(samplerStateAnisotropic16X);
};

void TextureManager::ReleaseTextures(){
	for(UINT i = 0; i < TM_NUM_TEXTURES; i++){
		ReleaseMacro(textures[i].resourceView);
	}
	textures.clear();
	delete &textures;
};
