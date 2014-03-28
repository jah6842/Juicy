#include "TextureManager.h"

TextureManager::TextureManager(){
	_samplerStatePoint = nullptr;
	_samplerStateLinear = nullptr;
	_samplerStateAnisotropic1X = nullptr;
	_samplerStateAnisotropic2X = nullptr;
	_samplerStateAnisotropic4X = nullptr;
	_samplerStateAnisotropic8X = nullptr;
	_samplerStateAnisotropic16X = nullptr;
	_samplerStateText = nullptr;
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
	deviceContext->PSSetShaderResources(type,1,&_textures[t].resourceView);
};

void TextureManager::SetActiveFilterMode(TM_FILTER_MODE m){
	
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();
	
	switch(m){
	case TM_FILTER_POINT:
		deviceContext->PSSetSamplers(0,1,&_samplerStatePoint);
		break;
	case TM_FILTER_LINEAR:
		deviceContext->PSSetSamplers(0,1,&_samplerStateLinear);
		break;
	case TM_FILTER_ANISO_1X:
		deviceContext->PSSetSamplers(0,1,&_samplerStateAnisotropic1X);
		break;
	case TM_FILTER_ANISO_2X:
		deviceContext->PSSetSamplers(0,1,&_samplerStateAnisotropic2X);
		break;
	case TM_FILTER_ANISO_4X:
		deviceContext->PSSetSamplers(0,1,&_samplerStateAnisotropic4X);
		break;
	case TM_FILTER_ANISO_8X:
		deviceContext->PSSetSamplers(0,1,&_samplerStateAnisotropic8X);
		break;
	case TM_FILTER_ANISO_16X:
		deviceContext->PSSetSamplers(0,1,&_samplerStateAnisotropic16X);
		break;
	case TM_FILTER_TEXT:
		deviceContext->PSSetSamplers(0,1,&_samplerStateText);
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
    sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sampDesc.MinLOD = 0.0f;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the Point Filtering Sample State
	HR(device->CreateSamplerState( &sampDesc, &_samplerStateText ));

	// Create the Point Filtering Sample State
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	HR(device->CreateSamplerState( &sampDesc, &_samplerStatePoint ));

	// Create the Linear Filtering Sample State
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	HR(device->CreateSamplerState( &sampDesc, &_samplerStateLinear ));

	// Create the 1X Anisotropic Filtering Sample State
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.MaxAnisotropy = 1;
	HR(device->CreateSamplerState( &sampDesc, &_samplerStateAnisotropic1X ));

	// Create the 2X Anisotropic Filtering Sample State
	sampDesc.MaxAnisotropy = 2;
	HR(device->CreateSamplerState( &sampDesc, &_samplerStateAnisotropic2X ));
	
	// Create the 4X Anisotropic Filtering Sample State
	sampDesc.MaxAnisotropy = 4;
	HR(device->CreateSamplerState( &sampDesc, &_samplerStateAnisotropic4X ));

	// Create the 8X Anisotropic Filtering Sample State
	sampDesc.MaxAnisotropy = 8;
	HR(device->CreateSamplerState( &sampDesc, &_samplerStateAnisotropic8X ));

	// Create the 16X Anisotropic Filtering Sample State
	sampDesc.MaxAnisotropy = 16;
	HR(device->CreateSamplerState( &sampDesc, &_samplerStateAnisotropic16X ));
};

// Load all of the textures that we're going to need
void TextureManager::InitTextures(){

	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	for(UINT i = 0; i < TM_NUM_TEXTURES; i++){
		// Set up a texture struct
		TEXTURE_DESCRIPTION desc = TEXTURE_DESCRIPTIONS[i];
		Texture tempTex;
		
		switch(desc.textureType){
			case TM_TYPE_DIFFUSE: tempTex.isDiffuseMap = true; break;
			case TM_TYPE_NORMAL: tempTex.isNormalMap = true; break;
			case TM_TYPE_SPECULAR: tempTex.isSpecularMap = true; break;
			case TM_TYPE_ILLUMINATION: tempTex.isIlluminationMap = true; break;
			case TM_TYPE_CUBEMAP: tempTex.isCubeMap = true; break;
		}

		tempTex.name = desc.textureName;
		tempTex.textureID = i;

		std::wstring wTexturePath = TEXTURE_PATH;
		wTexturePath += desc.textureName;

		// DirectXTK Texture Loading
		switch(desc.fileType){
			case TM_FILE_FORMAT_WIC:
				HR(CreateWICTextureFromFile(device, deviceContext, wTexturePath.c_str(), NULL, &tempTex.resourceView));
				break;
			case TM_FILE_FORMAT_DDS:
				HR(CreateDDSTextureFromFile(device, wTexturePath.c_str(), NULL, &tempTex.resourceView));
				break;
			case TM_FILE_FORMAT_DDS_CUBEMAP:
				HR(CreateDDSTextureFromFileEx(device, wTexturePath.c_str(), 1, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
					0, 0, true, NULL, &tempTex.resourceView));
				break;
		}

		// Add the texture to the list
		_textures[i] = tempTex;
	}
};

void TextureManager::ReleaseSamplerStates(){
	ReleaseMacro(_samplerStateText);
	ReleaseMacro(_samplerStatePoint);
	ReleaseMacro(_samplerStateLinear);
	ReleaseMacro(_samplerStateAnisotropic1X);
	ReleaseMacro(_samplerStateAnisotropic2X);
	ReleaseMacro(_samplerStateAnisotropic4X);
	ReleaseMacro(_samplerStateAnisotropic8X);
	ReleaseMacro(_samplerStateAnisotropic16X);
};

void TextureManager::ReleaseTextures(){
	for(UINT i = 0; i < TM_NUM_TEXTURES; i++){
		ReleaseMacro(_textures[i].resourceView);
	}
	_textures.clear();
	delete &_textures;
};
