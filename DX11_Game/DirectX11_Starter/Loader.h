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
	bool isInstanced;

	VertexShader(){}
	VertexShader(ID3D11VertexShader* v, ID3D11InputLayout* i, VSHADER id, bool b) : 
		vShader(v), vShaderInputLayout(i), vShaderID(id), isInstanced(b){}
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

struct ConstantBuffer {
	ID3D11Buffer*	cBuffer;
	CBUFFER_LAYOUT	layout;
	UINT			slot;

	ConstantBuffer(){}
	ConstantBuffer(ID3D11Buffer* b, CBUFFER_LAYOUT l, UINT s) :
		cBuffer(b), layout(l), slot(s){}
	~ConstantBuffer(){
		// Make sure to release resources when the pointer gets deleted
		ReleaseMacro(cBuffer);
	}
};


// Returns a VertexShader struct
std::shared_ptr<VertexShader> LoadVertexShader(ID3D11Device* device, VSHADER vShaderID);

// Returns a pixel shader pointer
std::shared_ptr<PixelShader> LoadPixelShader(ID3D11Device* device, PSHADER pShaderID);

// Returns the desired constant buffer
std::shared_ptr<ConstantBuffer> LoadConstantBuffer(ID3D11Device* device, CBUFFER_LAYOUT layout);

// Keep track of which shaders we've already loaded
static std::map<PSHADER, std::shared_ptr<PixelShader>> pShaders;
static std::map<VSHADER, std::shared_ptr<VertexShader>> vShaders;
static std::map<CBUFFER_LAYOUT, std::shared_ptr<ConstantBuffer>> cBuffers;

static void ReleaseShaders();

#endif // _LOADER_H