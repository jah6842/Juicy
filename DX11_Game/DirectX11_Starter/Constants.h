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
	VSHADER_TEXTURED_LIT_INSTANCED,
	VSHADER_SKYBOX,
	VSHADER_TEXT
};
const WCHAR* vShaderNames[]; // defined in CPP

// Pixel shaders
enum PSHADER{
	PSHADER_COLORED,
	PSHADER_TEXTURED,
	PSHADER_TEXTURED_INSTANCED,
	PSHADER_TEXTURED_LIT_INSTANCED,
	PSHADER_SKYBOX,
	PSHADER_TEXT
};
const WCHAR* pShaderNames[]; // defined in CPP

#pragma endregion

#pragma region Textures

// Textures
enum TM_TEXTURE {
	// Defaults
	TM_TEXTURE_DEFAULT,
	TM_TEXTURE_MARBLE,
	TM_TEXTURE_SAND,
	TM_TEXTURE_SCALES,
	// Frigate
	TM_TEXTURE_FRIGATE_DIFFUSE,
	TM_TEXTURE_FRIGATE_BUMP,
	TM_TEXTURE_FRIGATE_SPECULAR,
	TM_TEXTURE_FRIGATE_ILLUMINATION,
	// Cubemaps
	TM_TEXTURE_SPACE_CUBEMAP,
	TM_TEXTURE_EARTH_CUBEMAP,
	// Text
	TM_TEXTURE_TEXT,
	//enemy
	TM_TEXTURE_INVADER,

	TM_NUM_TEXTURES
};

enum TM_FILTER_MODE {
	TM_FILTER_POINT,
	TM_FILTER_LINEAR,
	TM_FILTER_ANISO_1X,
	TM_FILTER_ANISO_2X,
	TM_FILTER_ANISO_4X,
	TM_FILTER_ANISO_8X,
	TM_FILTER_ANISO_16X,
	TM_FILTER_TEXT,
};

enum TM_TEXTURE_TYPE {
	TM_TYPE_DIFFUSE,
	TM_TYPE_NORMAL,
	TM_TYPE_SPECULAR,
	TM_TYPE_ILLUMINATION,
	TM_TYPE_CUBEMAP
};

enum TM_FILE_FORMAT {
	TM_FILE_FORMAT_WIC,			// Use WIC for BMP, JPEG, PNG, TIFF, GIF 
	TM_FILE_FORMAT_DDS,			// Use for .dds
	TM_FILE_FORMAT_DDS_CUBEMAP	// Use for .dds cubemaps
};

struct TEXTURE_DESCRIPTION {
	std::wstring textureName;
	TM_TEXTURE_TYPE textureType;
	TM_FILE_FORMAT fileType;
};
const extern TEXTURE_DESCRIPTION TEXTURE_DESCRIPTIONS[TM_NUM_TEXTURES]; // defined in CPP

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
	MESH_SPHERE,
	MESH_FRIGATE
};
const WCHAR* meshNames[]; // defined in CPP

#pragma endregion

#pragma region Materials

enum MATERIALS {
	MATERIAL_DEFAULT,
	MATERIAL_MARBLE,
	MATERIAL_COLORED,
	MATERIAL_FRIGATE,
	MATERIAL_INVADER,
	MATERIAL_SKYBOX,
	MATERIAL_TEXT,

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

const extern MATERIAL_DESCRIPTION MATERIAL_DESCRIPTIONS[NUM_MATERIALS]; // defined in CPP

#pragma endregion

#endif // _CONSTANTS_H