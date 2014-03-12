#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

#include <d3d11.h>
#include <string>
#include <iostream>
#include "Utilities.h"
#include "Texture.h"

// Textures
enum TEXTURE_NAME {
	TM_TEXTURE_MARBLE,
	TM_TEXTURE_SAND,
	TM_TEXTURE_SCALES
};

class TextureManager {
public:
	TextureManager();
	~TextureManager();

	void Init();
private:

	void InitSamplerStates();
	void LoadTextures();

	ID3D11SamplerState* samplerStatePoint;
	ID3D11SamplerState* samplerStateLinear;
	ID3D11SamplerState* samplerStateAnisotropic1X;
	ID3D11SamplerState* samplerStateAnisotropic2X;
	ID3D11SamplerState* samplerStateAnisotropic4X;
	ID3D11SamplerState* samplerStateAnisotropic8X;
	ID3D11SamplerState* samplerStateAnisotropic16X;
};

#endif // _TEXTURE_MANAGER_H