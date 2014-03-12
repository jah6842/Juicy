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
