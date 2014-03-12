#include "TextureManager.h"

// Matches with enum in header
const WCHAR* textureNames[] = { 
	L"marble.png", 
	L"sand.jpg",
	L"scales.png"
};

TextureManager::TextureManager(){

};

TextureManager::~TextureManager(){

};

void TextureManager::Init(){
	InitSamplerStates();
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
void TextureManager::LoadTextures(){

};
