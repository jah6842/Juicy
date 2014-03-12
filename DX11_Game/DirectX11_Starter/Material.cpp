#include "Material.h"

// Static variables
//std::list<Material*> Material::_materials;

// Static class to cleanup all necessary items
void Material::Cleanup(){
	//typedef std::list<Material*>::iterator matItr;
	//for(matItr iterator = _materials.begin(); iterator != _materials.end(); iterator++) {
	//	//LOG(L"Deleting material: ", (*iterator)->_materialName);
	//	delete *iterator;
	//}
};

Material* Material::GetMaterial(MATERIAL_DESCRIPTION description){
	// Check if the material already exists
	//for(std::list<Material*>::iterator itr = _materials.begin(); itr != _materials.end(); itr++){
	//	if((*itr)->Compare(description)){
	//		return *itr;
	//	}
	//}

	//LOG(L"New material created: ", description.materialName);
	return new Material(description);
};

// Compare two shaders, returns a boolean if they are the same
bool Material::Compare(MATERIAL_DESCRIPTION description){
	
	if(//_materialName == description.materialName &&
		diffuseTexture == description.diffuseTexture &&
		textureFilter == description.textureFilter){
		return true;
	}

	return false;
};

// Constructor
Material::Material(MATERIAL_DESCRIPTION description){
	_materialName = description.materialName;
	diffuseTexture = description.diffuseTexture;
	textureFilter = description.textureFilter;

	// Get the current device
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	// Load the constant buffer
	_constantBuffer = LoadConstantBuffer(device, description.cBufferLayout);

	// Get the vertex shader
	_vertexShader = LoadVertexShader(device, description.vShaderID);

	// Get the pixel shader
	_pixelShader = LoadPixelShader(device, description.pShaderID);
};

Material::~Material(){
	// Most releases are handled by the Material Cleanup() function called when the application exits.
};

void Material::SetConstantBufferData(XMFLOAT4X4 world){
	// Get the current device context
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	// Per model data
	if(_constantBuffer->layout == CONSTANT_BUFFER_LAYOUT_PER_MODEL){
		CONSTANT_BUFFER_PER_MODEL modelData;
		modelData.world = world;

		// Update the constant buffer
		deviceContext->UpdateSubresource(_constantBuffer->cBuffer, 0, NULL, &modelData, 0, 0);
	}
};
