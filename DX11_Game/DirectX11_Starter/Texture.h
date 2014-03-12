#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <d3d11.h>
#include <string>

class Texture {
public:
	Texture();
	~Texture();

	std::wstring name; // Name of the texture
	UINT textureID; // Matches enum value
	ID3D11ShaderResourceView* resourceView; // Interface to data

	bool isDiffuseTexture;
	bool isNormalMap;
	bool isBumpMap;
};

#endif // _TEXTURE_H
