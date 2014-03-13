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
	L"marble.png", 
	L"sand.jpg",
	L"scales.png"
};

const WCHAR* meshNames[] = { 
	L"Cube", 
};


MATERIAL_DESCRIPTION MATERIAL_DESCRIPTIONS[] = {
	// PUT MATERIALS HERE
	{ 
		L"Default",
		MATERIAL_DEFAULT,
		VSHADER_TEXTURED_LIT_INSTANCED,
		PSHADER_TEXTURED_LIT_INSTANCED,
		TM_TEXTURE_MARBLE,
		TM_FILTER_ANISO_4X,
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
};