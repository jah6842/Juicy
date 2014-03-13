#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define SHADER_PATH L"../Resources/Shaders/";
#define TEXTURE_PATH L"../Resources/Textures/";
#define MODEL_PATH "../Resources/Models/";

#include "Utilities.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma region Shaders

// Vertex shaders
enum VSHADER{
	VSHADER_COLORED,
	VSHADER_TEXTURED,
	VSHADER_TEXTURED_INSTANCED,
	VSHADER_TEXTURED_LIT_INSTANCED
};
const WCHAR* vShaderNames[]; // defined in CPP

// Pixel shaders
enum PSHADER{
	PSHADER_COLORED,
	PSHADER_TEXTURED,
	PSHADER_TEXTURED_INSTANCED,
	PSHADER_TEXTURED_LIT_INSTANCED
};
const WCHAR* pShaderNames[]; // defined in CPP

#pragma endregion

#pragma region Textures

// Textures
enum TM_TEXTURE {
	TM_TEXTURE_DEFAULT,
	TM_TEXTURE_MARBLE,
	TM_TEXTURE_SAND,
	TM_TEXTURE_SCALES,
	TM_NUM_TEXTURES
};
const WCHAR* textureNames[]; // defined in CPP

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

#pragma endregion

#pragma region Constant Buffers

// CONSTANT BUFFERS MUST BE IN MULTIPLES OF 16 BYTES!
// CONSTANT BUFFERS MUST BE IN MULTIPLES OF 16 BYTES!
// Add padding where necessary.
// Did I mention?: CONSTANT BUFFERS MUST BE IN MULTIPLES OF 16 BYTES!

enum CBUFFER_LAYOUT {
	CONSTANT_BUFFER_LAYOUT_PER_FRAME,
	CONSTANT_BUFFER_LAYOUT_PER_MODEL,
	CONSTANT_BUFFER_LAYOUT_DIRECTIONAL_LIGHT
};

// CBuffer slot 0
struct CONSTANT_BUFFER_PER_FRAME {
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT4X4 viewProj;
};

// CBuffer slot 1
struct CONSTANT_BUFFER_PER_MODEL {
	XMFLOAT4X4 world;
};

// CBuffer slot 2
struct CONSTANT_BUFFER_DIRECTIONAL_LIGHT {
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;
	float padding;
};

#pragma endregion

#pragma region Meshes

// Vertex shaders
enum MESHES{
	MESH_CUBE,
	MESH_SHIP
};
extern const WCHAR* meshNames[]; // defined in CPP

#pragma endregion

enum MATERIALS {
	MATERIAL_DEFAULT,
	MATERIAL_MARBLE,
	MATERIAL_COLORED,
	// Add more materials here
	NUM_MATERIALS
};

// A description of a material to be created
struct MATERIAL_DESCRIPTION {
	std::wstring materialName;
	MATERIALS materialID;
	VSHADER vShaderID;
	PSHADER pShaderID;
	TM_TEXTURE diffuseTexture;
	TM_FILTER_MODE textureFilter;
	CBUFFER_LAYOUT cBufferLayout;
};

extern MATERIAL_DESCRIPTION MATERIAL_DESCRIPTIONS[]; // defined in CPP

#endif // _CONSTANTS_H