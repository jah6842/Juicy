#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

#define TEXTURE_PATH L"../Resources/Textures/";

#include <d3d11.h>
#include <WICTextureLoader.h>
#include <string>
#include <iostream>
#include <map>
#include "Utilities.h"

using namespace DirectX;

// Textures
enum TM_TEXTURE {
	TM_TEXTURE_MARBLE,
	TM_TEXTURE_SAND,
	TM_TEXTURE_SCALES,
	TM_NUM_TEXTURES
};

enum TM_FILTER_MODE {
	TM_FILTER_POINT,
	TM_FILTER_LINEAR,
	TM_FILTER_ANISO_1X,
	TM_FILTER_ANISO_2X,
	TM_FILTER_ANISO_4X,
	TM_FILTER_ANISO_8X,
	TM_FILTER_ANISO_16X
};

enum TM_TEXTURE_TYPE {
	TM_TYPE_DIFFUSE,
	TM_TYPE_NORMAL,
	TM_TYPE_SPECULAR
};

struct Texture {
	std::wstring name; // Name of the texture
	UINT textureID; // Matches enum value
	ID3D11ShaderResourceView* resourceView; // Interface to data

	bool isDiffuseMap;
	bool isNormalMap; // aka bump map
	bool isSpecularMap; // For lighting
};

class TextureManager {
public:
	TextureManager();
	~TextureManager();

	void Init();
	void Cleanup();

	void SetActiveTexture(TM_TEXTURE t, TM_TEXTURE_TYPE type = TM_TYPE_DIFFUSE);
	void SetActiveFilterMode(TM_FILTER_MODE m);
private:

	void InitSamplerStates();
	void LoadTextures();

	// <TM_TEXTURE, Texture> container
	std::map<UINT, Texture> textures;

	// Filter modes that we can use
	ID3D11SamplerState* samplerStatePoint;
	ID3D11SamplerState* samplerStateLinear;
	ID3D11SamplerState* samplerStateAnisotropic1X;
	ID3D11SamplerState* samplerStateAnisotropic2X;
	ID3D11SamplerState* samplerStateAnisotropic4X;
	ID3D11SamplerState* samplerStateAnisotropic8X;
	ID3D11SamplerState* samplerStateAnisotropic16X;
};

#endif // _TEXTURE_MANAGER_H