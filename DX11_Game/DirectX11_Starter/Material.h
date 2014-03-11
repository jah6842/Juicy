#ifndef _MATERIAL_H
#define _MATERIAL_H

// A path to search for resources relative to the .exe
#define SHADER_PATH L"../Resources/Shaders/";
#define TEXTURE_PATH L"../Resources/Textures/";

#include <map>
#include <string>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Utilities.h"
#include "ConstantBuffer.h"
#include "Vertex.h"
#include <iostream>
#include <list>
#include "Camera.h"

using namespace DirectX;

// Vertex shaders
enum {
	VSHADER_COLORED,
	VSHADER_TEXTURED,
	VSHADER_TEXTURED_INSTANCED,
	VSHADER_TEXTURED_LIT_INSTANCED
};

// Pixel shaders
enum {
	PSHADER_COLORED,
	PSHADER_TEXTURED,
	PSHADER_TEXTURED_INSTANCED,
	PSHADER_TEXTURED_LIT_INSTANCED
};

// Textures
enum {
	TEXTURE_MARBLE_PNG,
	TEXTURE_SAND_JPG,
	TEXTURE_SCALES_PNG
};

// A description of a material to be created
struct MATERIAL_DESCRIPTION {
	std::wstring materialName;
	UINT vShaderID;
	UINT pShaderID;
	UINT diffuseTextureID;
	CONSTANT_BUFFER_LAYOUT cBufferLayout;
};

class Material {

public:
	// Gets a material from created shaders if one exists,
	// if not a new shader is created and added to the list
	static std::list<Material*> _materials;
	static Material* GetMaterial(MATERIAL_DESCRIPTION description);
	bool Compare(MATERIAL_DESCRIPTION description);

	// Assumes vertex and pixel shaders have the same prefix, optional texture
	Material(MATERIAL_DESCRIPTION description);

	// Destructor
	~Material();

	void SetConstantBufferData(XMFLOAT4X4 world);
	void SetInputAssemblerOptions();
	bool IsInstanced();

	// Cleanup all of our static objects
	static void Cleanup();

	// Call once per frame to clear cached options
	static void ClearOptions();

private:

	static const WCHAR* textureNames[];
	static const WCHAR* vShaderNames[];
	static const WCHAR* pShaderNames[];

	// Static containers so we don't end up with duplicates
	static std::map<UINT, ID3D11VertexShader*> _vertexShaders;
	static std::map<UINT, ID3D11PixelShader*> _pixelShaders;
	static std::map<UINT, ID3D11InputLayout*> _inputLayouts;
	static std::map<UINT, ID3D11ShaderResourceView*> _textures;
	static std::map<UINT, ID3D11SamplerState*> _textureSamplers;
	
	static ID3D11SamplerState* currentTextureSampler;
	static ID3D11ShaderResourceView* currentTexture;
	static ID3D11InputLayout* currentInputLayout;
	static ID3D11VertexShader* currentVertexShader;
	static ID3D11PixelShader* currentPixelShader;
	static ID3D11Buffer* currentConstantBuffer;

	void LoadVertexShader(UINT vShaderID);
	void LoadPixelShader(UINT pShaderID);
	void LoadConstantBuffer(CONSTANT_BUFFER_LAYOUT layout);
	void LoadTexture(UINT textureID);

	std::wstring _materialName;

	UINT _vShaderID;
	UINT _pShaderID;
	ID3D11PixelShader* _pixelShader;
	ID3D11VertexShader* _vertexShader;

	UINT _diffuseTextureID;
	ID3D11ShaderResourceView* _diffuseTexture;
	ID3D11SamplerState* _diffuseTextureSamplerState;

	ID3D11InputLayout* _inputLayout;
	ID3D11Buffer* _vsConstantBuffer;
	CONSTANT_BUFFER_LAYOUT _cBufferLayout;

	bool _isInstanced;
};

#endif // _MATERIAL_H