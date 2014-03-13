#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <d3d11.h>
#include "Utilities.h"
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include "Loader.h"

using namespace DirectX;

// A description of a material to be created
struct MATERIAL_DESCRIPTION {
	std::wstring materialName;
	VSHADER vShaderID;
	PSHADER pShaderID;
	TM_TEXTURE diffuseTexture;
	TM_FILTER_MODE textureFilter;
	CBUFFER_LAYOUT cBufferLayout;
};

class Material {

public:
	bool Compare(MATERIAL_DESCRIPTION description);
	bool Compare(Material*);

	// Assumes vertex and pixel shaders have the same prefix, optional texture
	Material(MATERIAL_DESCRIPTION description);
	// Destructor
	~Material();

	std::wstring materialName;
	TM_TEXTURE diffuseTexture;
	TM_FILTER_MODE textureFilter;
	std::shared_ptr<PixelShader> pixelShader;
	std::shared_ptr<VertexShader> vertexShader;
	std::shared_ptr<ConstantBuffer> constantBuffer;
};

#endif // _MATERIAL_H