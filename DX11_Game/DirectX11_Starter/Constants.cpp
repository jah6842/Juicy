#include "Constants.h"

const WCHAR* vShaderNames[] = {
	L"ColoredVertex.cso",
	L"TexturedVertex.cso",
	L"TexturedInstancedVertex.cso",
	L"TexturedInstancedLightingVertex.cso"
};

const WCHAR* pShaderNames[] = {
	L"ColoredPixel.cso",
	L"TexturedPixel.cso",
	L"TexturedInstancedPixel.cso",
	L"TexturedInstancedLightingPixel.cso"
};

const WCHAR* textureNames[TM_NUM_TEXTURES] = { 
	L"default.png",
	L"marble.png", 
	L"sand.jpg",
	L"scales.png",
	L"frigate/space_frigate_6_color.png",
	L"frigate/space_frigate_6_bump.gif",
	L"frigate/space_frigate_6_specular.png",
	L"frigate/space_frigate_6_illumination.png",
};

const WCHAR* meshNames[] = { 
	L"Cube",
	L"frigate.fbx",
};

MATERIAL_DESCRIPTION MATERIAL_DESCRIPTIONS[] = {
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
};