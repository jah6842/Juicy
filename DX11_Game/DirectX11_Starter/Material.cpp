#include "Material.h"

// Static variables
std::list<Material*> Material::_materials;

ID3D11PixelShader* Material::currentPixelShader = nullptr;
ID3D11VertexShader* Material::currentVertexShader = nullptr;
ID3D11InputLayout* Material::currentInputLayout = nullptr;
ID3D11Buffer* Material::currentConstantBuffer = nullptr;

// Static class to cleanup all necessary items
void Material::Cleanup(){
	typedef std::list<Material*>::iterator matItr;
	for(matItr iterator = _materials.begin(); iterator != _materials.end(); iterator++) {
		LOG(L"Deleting material: ", (*iterator)->_materialName);
		delete *iterator;
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
		diffuseTexture == description.diffuseTexture &&
		textureFilter == description.textureFilter && 
		_cBufferLayout == description.cBufferLayout){
		return true;
	}

	return false;
};

// Constructor
Material::Material(MATERIAL_DESCRIPTION description){
	_isInstanced = false;
	_materialName = description.materialName;
	_cBufferLayout = description.cBufferLayout;

	diffuseTexture = description.diffuseTexture;
	textureFilter = description.textureFilter;

	// Load the vertex shader
	LoadShaders(description.vShaderID, description.pShaderID);

	// Load the constant buffer
	LoadConstantBuffer(description.cBufferLayout);

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
	if(_vertexShader->vShader != currentVertexShader){
		currentVertexShader = _vertexShader->vShader;
		deviceContext->VSSetShader(_vertexShader->vShader, NULL, 0);
	}
	if(_vertexShader->vShaderInputLayout != currentInputLayout){
		currentInputLayout = _vertexShader->vShaderInputLayout;
		deviceContext->IASetInputLayout(_vertexShader->vShaderInputLayout);
	}
	if(_vsConstantBuffer != currentConstantBuffer){
		deviceContext->VSSetConstantBuffers(1, 1, &_vsConstantBuffer);
		currentConstantBuffer = _vsConstantBuffer;
	}
	if( _pixelShader->pShader != currentPixelShader){
		currentPixelShader =_pixelShader->pShader;
		deviceContext->PSSetShader(_pixelShader->pShader, NULL, 0);
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

void Material::LoadShaders(VSHADER vShader, PSHADER pShader){
	// Get the current device
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	// Get the pixel shader
	_pixelShader = LoadPixelShader(device, pShader);

	// Get the vertex shader
	_vertexShader = LoadVertexShader(device, vShader);

	// Move this to VertexShader structure
	_isInstanced = true;
};

bool Material::IsInstanced(){
	return _isInstanced;
};

void Material::ClearOptions(){
	currentPixelShader = nullptr;
	currentVertexShader = nullptr;
	currentInputLayout = nullptr;
	currentConstantBuffer = nullptr;
};