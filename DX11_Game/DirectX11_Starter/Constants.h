#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define SHADER_PATH L"../Resources/Shaders/";

#include "Utilities.h"

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

#endif // _CONSTANTS_H