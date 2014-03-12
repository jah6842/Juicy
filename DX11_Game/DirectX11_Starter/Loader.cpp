#include "Loader.h"

// Returns a VertexShader struct for the desired shader
std::shared_ptr<VertexShader> LoadVertexShader(ID3D11Device* device, VSHADER vShaderID){

	if(vShaders[vShaderID] != nullptr){
		return vShaders[vShaderID];
	}

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;

	LOG(L"Creating vertex shader: ", std::to_wstring(vShaderID));

	// Load shader from file
	ID3DBlob* vsBlob;
	std::wstring vShaderPath = SHADER_PATH;
	vShaderPath += vShaderNames[vShaderID];
	D3DReadFileToBlob(vShaderPath.c_str(), &vsBlob);

	// Create the shader on the device
	HR(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&vertexShader));
	
	// Create an input description
	D3D11_INPUT_ELEMENT_DESC* description = nullptr;
	UINT descriptionSize = 0;

	switch(vShaderID){
	case VSHADER_TEXTURED_LIT_INSTANCED:
	case VSHADER_TEXTURED_INSTANCED:
		description = VERTEX_DESCRIPTION_ALL;
		descriptionSize = 8;
		break;
	default:
		LOG(L"UNSUPPORTED SHADER!");
		assert(description != nullptr);
		break;
	}

	// Before cleaning up the data, create the input layout
	HR(device->CreateInputLayout(
		description,
		descriptionSize,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout));

	// Clean up
	ReleaseMacro(vsBlob);

	std::shared_ptr<VertexShader> v (new VertexShader(vertexShader, inputLayout, vShaderID));
	vShaders[vShaderID] = v;

	return vShaders[vShaderID];
};

// Returns a pixel shader pointer for the desired shader
std::shared_ptr<PixelShader> LoadPixelShader(ID3D11Device* device, PSHADER pShaderID){

	if(pShaders[pShaderID] != nullptr){
		return pShaders[pShaderID];
	}

	LOG(L"Creating pixel shader: ", std::to_wstring(pShaderID));

	ID3D11PixelShader* pixelShader = nullptr;

	// Load shader from file
	ID3DBlob* psBlob;
	std::wstring pShaderPath = SHADER_PATH;
	pShaderPath += pShaderNames[pShaderID];
	D3DReadFileToBlob(pShaderPath.c_str(), &psBlob);

	// Create the shader on the device
	HR(device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&pixelShader));

	// Clean up
	ReleaseMacro(psBlob);

	std::shared_ptr<PixelShader> p (new PixelShader(pixelShader, pShaderID));
	pShaders[pShaderID] = p;

	return pShaders[pShaderID];
};
