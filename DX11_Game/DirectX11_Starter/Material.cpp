#include "Material.h"

// Texture names (SHOULD BE IN THE SAME ORDER AS THE TEXTURE ENUM IN THE HEADER!)
const WCHAR* Material::textureNames[] = { 
	L"marble.png", 
	L"sand.jpg",
	L"scales.png" 
};

const WCHAR* Material::vShaderNames[] = {
	L"ColoredVertex.cso",
	L"TexturedVertex.cso",
	L"TexturedInstancedVertex.cso"
};

const WCHAR* Material::pShaderNames[] = {
	L"ColoredPixel.cso",
	L"TexturedPixel.cso",
	L"TexturedInstancedPixel.cso"
};

// Static variables
std::list<Material*> Material::_materials;
std::map<UINT, ID3D11PixelShader*> Material::_pixelShaders;
std::map<UINT, ID3D11VertexShader*> Material::_vertexShaders;
std::map<UINT, ID3D11InputLayout*> Material::_inputLayouts;
std::map<UINT, ID3D11ShaderResourceView*> Material::_textures;
std::map<UINT, ID3D11SamplerState*> Material::_textureSamplers;

ID3D11PixelShader* Material::currentPixelShader = nullptr;
ID3D11VertexShader* Material::currentVertexShader = nullptr;
ID3D11InputLayout* Material::currentInputLayout = nullptr;
ID3D11ShaderResourceView* Material::currentTexture = nullptr;
ID3D11SamplerState* Material::currentTextureSampler = nullptr;
ID3D11Buffer* Material::currentConstantBuffer = nullptr;

// Static class to cleanup all necessary items
void Material::Cleanup(){
	typedef std::list<Material*>::iterator matItr;
	for(matItr iterator = _materials.begin(); iterator != _materials.end(); iterator++) {
		LOG(L"Deleting material: ", (*iterator)->_materialName);
		delete *iterator;
	}
	typedef std::map<UINT, ID3D11PixelShader*>::iterator pixelItr;
	for(pixelItr iterator = _pixelShaders.begin(); iterator != _pixelShaders.end(); iterator++) {
		ReleaseMacro(iterator->second);
		LOG(L"  Released Pixel Shader: ", std::to_wstring(iterator->first));
	}
	typedef std::map<UINT, ID3D11VertexShader*>::iterator vertexItr;
	for(vertexItr iterator = _vertexShaders.begin(); iterator != _vertexShaders.end(); iterator++) {
		ReleaseMacro(iterator->second);
		LOG(L"  Released Vertex Shader: ", std::to_wstring(iterator->first));
	}
	typedef std::map<UINT, ID3D11InputLayout*>::iterator inputItr;
	for(inputItr iterator = _inputLayouts.begin(); iterator != _inputLayouts.end(); iterator++) {
		ReleaseMacro(iterator->second);
		LOG(L"  Released Input Layout: ", std::to_wstring(iterator->first));
	}
	typedef std::map<UINT, ID3D11ShaderResourceView*>::iterator texItr;
	for(texItr iterator = _textures.begin(); iterator != _textures.end(); iterator++) {
		ReleaseMacro(iterator->second);
		LOG(L"  Released Shader Resource View: ", std::to_wstring(iterator->first));
	}
	typedef std::map<UINT, ID3D11SamplerState*>::iterator texSampItr;
	for(texSampItr iterator = _textureSamplers.begin(); iterator != _textureSamplers.end(); iterator++) {
		ReleaseMacro(iterator->second);
		LOG(L"  Released Sampler State: ", std::to_wstring(iterator->first));
	}
};

Material* Material::GetMaterial(MATERIAL_DESCRIPTION description){
	// Check if the material already exists
	for(std::list<Material*>::iterator itr = _materials.begin(); itr != _materials.end(); itr++){
		if((*itr)->Compare(description)){
			return *itr;
		}
	}

	LOG(L"New material created: ", description.materialName);
	return new Material(description);
};

// Compare two shaders, returns a boolean if they are the same
bool Material::Compare(MATERIAL_DESCRIPTION description){
	if(_materialName == description.materialName &&
		_vShaderID == description.vShaderID &&
		_pShaderID == description.pShaderID &&
		_diffuseTextureID == description.diffuseTextureID &&
		_cBufferLayout == description.cBufferLayout){
		return true;
	}

	return false;
};

// Constructor
Material::Material(MATERIAL_DESCRIPTION description){
	_isInstanced = false;
	_materialName = description.materialName;
	_vShaderID = description.vShaderID;
	_pShaderID = description.pShaderID;
	_cBufferLayout = description.cBufferLayout;
	_diffuseTextureID = description.diffuseTextureID;

	// Load the vertex shader
	LoadVertexShader(description.vShaderID);

	// Load the pixel shader
	LoadPixelShader(description.pShaderID);

	// Load the constant buffer
	LoadConstantBuffer(description.cBufferLayout);

	// Load textures
	LoadTexture(description.diffuseTextureID);

	_materials.push_back(this);
};

Material::~Material(){
	// Most releases are handled by the Material Cleanup() function called when the application exits.
	ReleaseMacro(_vsConstantBuffer);
};

void Material::SetConstantBufferData(XMFLOAT4X4 world){
	// Get the current device context
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	// Per model data
	if(_cBufferLayout == CONSTANT_BUFFER_LAYOUT_PER_MODEL){
		CONSTANT_BUFFER_PER_MODEL modelData;
		modelData.world = world;

		// Update the constant buffer itself
		deviceContext->UpdateSubresource(_vsConstantBuffer, 0, NULL, &modelData, 0, 0);
	}
};

void Material::SetInputAssemblerOptions(){
	// Get the current device context
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	// Check if we need to change state, if not use the current state.
	if(_vertexShader != currentVertexShader){
		currentVertexShader = _vertexShader;
		deviceContext->VSSetShader(_vertexShader, NULL, 0);
	}
	if(_vsConstantBuffer != currentConstantBuffer){
		deviceContext->VSSetConstantBuffers(1, 1, &_vsConstantBuffer);
		currentConstantBuffer = _vsConstantBuffer;
	}
	if(_pixelShader != currentPixelShader){
		currentPixelShader = _pixelShader;
		deviceContext->PSSetShader(_pixelShader, NULL, 0);
	}
	if(_diffuseTexture != currentTexture){
		currentTexture = _diffuseTexture;
		deviceContext->PSSetShaderResources(0,1,&_diffuseTexture);
	}
	if(_diffuseTextureSamplerState != currentTextureSampler){
		currentTextureSampler = _diffuseTextureSamplerState;
		deviceContext->PSSetSamplers(0,1,&_diffuseTextureSamplerState);
	}
	if(_inputLayout != currentInputLayout){
		currentInputLayout = _inputLayout;
		deviceContext->IASetInputLayout(_inputLayout);
	}
};

void Material::LoadConstantBuffer(CONSTANT_BUFFER_LAYOUT layout){
	// Get the current device
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	// Create a constant buffer
	D3D11_BUFFER_DESC cBufferDesc;

	switch(layout){
	case CONSTANT_BUFFER_LAYOUT_PER_MODEL:
		cBufferDesc.ByteWidth = sizeof(CONSTANT_BUFFER_PER_MODEL); break;
	default:
		LOG(L"INVALID CONSTANT BUFFER TYPE! ", __FILEW__, L", ", std::to_wstring(__LINE__));
	}

	cBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags		= 0;
	cBufferDesc.MiscFlags			= 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(
		&cBufferDesc,
		NULL,
		&_vsConstantBuffer));
};

void Material::LoadPixelShader(UINT pShaderID){

	// Check if the shader already exists
	if(_pixelShaders.count(pShaderID)){
		_pixelShader = _pixelShaders[pShaderID];
		return;
	}

	// Get the current device
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	// Load Pixel Shader ---------------------------------------
	ID3DBlob* psBlob;
	std::wstring pShaderPath = SHADER_PATH;
	pShaderPath += pShaderNames[pShaderID];
	D3DReadFileToBlob(pShaderPath.c_str(), &psBlob);

	// Create the shader on the device
	HR(device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&_pixelShader));

	// Clean up
	ReleaseMacro(psBlob);

	// Add it to the static list
	_pixelShaders[pShaderID] = _pixelShader;
};

// Loads shaders from compiled shader object (.cso) files, and uses the
// vertex shader to create an input layout which is needed when sending
// vertex data to the device
void Material::LoadVertexShader(UINT vShaderID){

	// Check if the shader already exists
	if(_vertexShaders.count(vShaderID)){
		_vertexShader = _vertexShaders[vShaderID];
		_inputLayout = _inputLayouts[vShaderID];
		_isInstanced = true;
		return;
	}

	LOG(L"Creating vertex shader: ", std::to_wstring(vShaderID));

	// Get the current device
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	std::wstring vShaderPath = SHADER_PATH;
	vShaderPath += vShaderNames[vShaderID];
	D3DReadFileToBlob(vShaderPath.c_str(), &vsBlob);

	// Create the shader on the device
	HR(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&_vertexShader));
	
	D3D11_INPUT_ELEMENT_DESC* description = nullptr;
	UINT descriptionSize = 0;

	switch(vShaderID){
	case VSHADER_TEXTURED_INSTANCED:
		description = VERTEX_DESCRIPTION_ALL;
		descriptionSize = 8;
		_isInstanced = true;
		break;
	default:
		LOG(L"UNSUPPORTED SHADER!");
		break;
	}

	assert(description != nullptr);
	assert(descriptionSize != 0);

	// Before cleaning up the data, create the input layout
	HR(device->CreateInputLayout(
		description,
		descriptionSize,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&_inputLayout));

	// Clean up
	ReleaseMacro(vsBlob);

	// Add it to the static list
	_vertexShaders[vShaderID] = _vertexShader;
	_inputLayouts[vShaderID] = _inputLayout;
};

void Material::LoadTexture(UINT textureID){
	// Check if the texture already exists
	if(_textures.count(textureID)){
		_diffuseTexture = _textures[textureID];
		_diffuseTextureSamplerState = _textureSamplers[textureID];
		return;
	}

	// Get the current device
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	std::wstring dTexturePath = TEXTURE_PATH;
	dTexturePath += textureNames[textureID];

	// NEW DirectXTK Texture Loading
	HR(CreateWICTextureFromFile(device, deviceContext, dTexturePath.c_str(), NULL, &_diffuseTexture));



	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.MaxAnisotropy = 8;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    
	//Create the Sample State
	device->CreateSamplerState( &sampDesc, &_diffuseTextureSamplerState );

	// Add it to the static list
	_textures[textureID] = _diffuseTexture;
	_textureSamplers[textureID] = _diffuseTextureSamplerState;
};

bool Material::IsInstanced(){
	return _isInstanced;
};