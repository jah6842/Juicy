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

class Material {

public:
	// Assumes vertex and pixel shaders have the same prefix, optional texture
	Material(MATERIALS mat);
	// Destructor
	~Material();

	std::wstring materialName;
	MATERIALS materialID;
	TM_TEXTURE diffuseTexture;
	TM_FILTER_MODE textureFilter;
	std::shared_ptr<PixelShader> pixelShader;
	std::shared_ptr<VertexShader> vertexShader;
	std::shared_ptr<ConstantBuffer> constantBuffer;

	static std::map<UINT, std::shared_ptr<Material>> allMaterials;
	static std::shared_ptr<Material> LoadMaterial(ID3D11Device* device, MATERIALS mat);
	static ID3D11Buffer* Create2DVertexBuffer(void* vertices, UINT numVertices, VERTEX_TYPE t);
};


#endif // _MATERIAL_H