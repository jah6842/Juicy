#ifndef _LOADER_H
#define _LOADER_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <map>
#include <memory>
#include "Vertex.h"
#include "Utilities.h"
#include "Constants.h"

struct VertexShader {
	ID3D11VertexShader* vShader;
	ID3D11InputLayout*  vShaderInputLayout;
	VSHADER vShaderID;

	VertexShader(){}
	VertexShader(ID3D11VertexShader* v, ID3D11InputLayout* i, VSHADER id) : 
		vShader(v), vShaderInputLayout(i), vShaderID(id){}
	~VertexShader(){ 
		// Make sure to release resources when the pointer gets deleted
		ReleaseMacro(vShader); 
		ReleaseMacro(vShaderInputLayout);
	}
};

struct PixelShader {
	ID3D11PixelShader* pShader;
	PSHADER pShaderID;

	PixelShader(){}
	PixelShader(ID3D11PixelShader* p, PSHADER id) :
		pShader(p), pShaderID(id){}
	~PixelShader(){ 
		// Make sure to release resources when the pointer gets deleted
		ReleaseMacro(pShader); 
	}
};

// Returns a VertexShader struct
std::shared_ptr<VertexShader> LoadVertexShader(ID3D11Device* device, VSHADER vShaderID);

// Returns a pixel shader pointer
std::shared_ptr<PixelShader> LoadPixelShader(ID3D11Device* device, PSHADER pShaderID);

// Keep track of which shaders we've already loaded
static std::map<PSHADER, std::shared_ptr<PixelShader>> pShaders;
static std::map<VSHADER, std::shared_ptr<VertexShader>> vShaders;

static void ReleaseShaders();

#endif // _LOADER_H