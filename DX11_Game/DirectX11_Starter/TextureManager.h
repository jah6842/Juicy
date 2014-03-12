#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

#include <d3d11.h>
#include <WICTextureLoader.h>
#include <string>
#include <iostream>
#include <map>
#include "Utilities.h"
#include "Constants.h"

using namespace DirectX;

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
	void InitTextures();
	void ReleaseSamplerStates();
	void ReleaseTextures();

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