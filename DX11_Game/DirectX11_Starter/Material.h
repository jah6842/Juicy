#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <d3d11.h>
#include "Utilities.h"
#include <iostream>
#include <list>
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
	//static std::list<Material*> _materials;
	static Material* GetMaterial(MATERIAL_DESCRIPTION description);
	bool Compare(MATERIAL_DESCRIPTION description);
	bool Compare(Material*);

	// Assumes vertex and pixel shaders have the same prefix, optional texture
	Material(MATERIAL_DESCRIPTION description);

	// Destructor
	~Material();

	void SetConstantBufferData(XMFLOAT4X4 world);

	// Cleanup all of our static objects
	static void Cleanup();

	std::wstring _materialName;
	TM_TEXTURE diffuseTexture;
	TM_FILTER_MODE textureFilter;
	std::shared_ptr<PixelShader> _pixelShader;
	std::shared_ptr<VertexShader> _vertexShader;
	std::shared_ptr<ConstantBuffer> _constantBuffer;

private:
	// Gets a material from created shaders if one exists,
	// if not a new shader is created and added to the list
	//static std::map< std::wstring, std::shared_ptr<Material>> _materials;
};

#endif // _MATERIAL_H