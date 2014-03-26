#include "Constants.h"

const WCHAR* vShaderNames[] = {
	L"ColoredVertex.cso",
	L"TexturedVertex.cso",
	L"TexturedInstancedVertex.cso",
	L"TexturedInstancedLightingVertex.cso",
	L"SkyboxVertex.cso"
};

const WCHAR* pShaderNames[] = {
	L"ColoredPixel.cso",
	L"TexturedPixel.cso",
	L"TexturedInstancedPixel.cso",
	L"TexturedInstancedLightingPixel.cso",
	L"SkyboxPixel.cso"
};

const TEXTURE_DESCRIPTION TEXTURE_DESCRIPTIONS[TM_NUM_TEXTURES] = {
	// NAME, TYPE, FILE FORMAT
	{ L"default.png" , TM_TYPE_DIFFUSE, TM_FILE_FORMAT_WIC },
	{ L"marble.png", TM_TYPE_DIFFUSE, TM_FILE_FORMAT_WIC },
	{ L"sand.jpg", TM_TYPE_DIFFUSE, TM_FILE_FORMAT_WIC },
	{ L"scales.png", TM_TYPE_DIFFUSE, TM_FILE_FORMAT_WIC },
	
	{ L"frigate/space_frigate_6_color.png", TM_TYPE_DIFFUSE, TM_FILE_FORMAT_WIC },
	{ L"frigate/space_frigate_6_bump.gif", TM_TYPE_NORMAL, TM_FILE_FORMAT_WIC },
	{ L"frigate/space_frigate_6_specular.png", TM_TYPE_SPECULAR, TM_FILE_FORMAT_WIC },
	{ L"frigate/space_frigate_6_illumination.png", TM_TYPE_ILLUMINATION, TM_FILE_FORMAT_WIC },

	{ L"spaceCubemap3.dds", TM_TYPE_CUBEMAP, TM_FILE_FORMAT_DDS_CUBEMAP },
	{ L"earth.dds", TM_TYPE_CUBEMAP, TM_FILE_FORMAT_DDS_CUBEMAP },
};

const WCHAR* meshNames[] = { 
	L"Cube",
	L"sphere.fbx",
	L"frigate.fbx",
};

const MATERIAL_DESCRIPTION MATERIAL_DESCRIPTIONS[NUM_MATERIALS] = {
	// PUT MATERIALS HERE
	{ 
		L"Default",
		MATERIAL_DEFAULT,
		VSHADER_TEXTURED_LIT_INSTANCED,
		PSHADER_TEXTURED_LIT_INSTANCED,
		TM_TEXTURE_DEFAULT,
		TM_FILTER_POINT,
		CONSTANT_BUFFER_LAYOUT_PER_MODEL 
	},
	{ 
		L"Marble",
		MATERIAL_MARBLE,
		VSHADER_TEXTURED_LIT_INSTANCED,
		PSHADER_TEXTURED_LIT_INSTANCED,
		TM_TEXTURE_MARBLE,
		TM_FILTER_ANISO_4X,
		CONSTANT_BUFFER_LAYOUT_PER_MODEL 
	},
	{ 
		L"Colored",
		MATERIAL_MARBLE,
		VSHADER_COLORED,
		PSHADER_COLORED,
		TM_TEXTURE_MARBLE,
		TM_FILTER_POINT,
		CONSTANT_BUFFER_LAYOUT_PER_MODEL 
	},
	{ 
		L"Frigate",
		MATERIAL_FRIGATE,
		VSHADER_TEXTURED_LIT_INSTANCED,
		PSHADER_TEXTURED_LIT_INSTANCED,
		TM_TEXTURE_FRIGATE_DIFFUSE,
		TM_FILTER_ANISO_4X,
		CONSTANT_BUFFER_LAYOUT_PER_MODEL 
	},
	{
		L"Skybox",
		MATERIAL_SKYBOX,
		VSHADER_SKYBOX,
		PSHADER_SKYBOX,
		TM_TEXTURE_SPACE_CUBEMAP,
		TM_FILTER_ANISO_4X,
		CONSTANT_BUFFER_LAYOUT_PER_MODEL 
	},
};