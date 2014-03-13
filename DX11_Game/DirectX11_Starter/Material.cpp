#include "Material.h"

// Compare two shaders, returns a boolean if they are the same
bool Material::Compare(MATERIAL_DESCRIPTION description){
	if( diffuseTexture == description.diffuseTexture &&
		textureFilter == description.textureFilter){
		return true;
	}
	return false;
};

bool Material::Compare(Material* mat){
	if( diffuseTexture == mat->diffuseTexture &&
		textureFilter == mat->textureFilter){
		return true;
	}
	return false;
};

static std::map<MATERIAL_DESCRIPTION, std::shared_ptr<Material>> _materials;
std::shared_ptr<Material> LoadMaterial(ID3D11Device* device, MATERIAL_DESCRIPTION desc){
	if(_materials[desc] != nullptr)
		return _materials[desc];

	std::shared_ptr<Material> m (new Material(desc));
	_materials[desc] = m;
	return _materials[desc];
};

// Constructor
Material::Material(MATERIAL_DESCRIPTION description){
	materialName = description.materialName;
	diffuseTexture = description.diffuseTexture;
	textureFilter = description.textureFilter;

	// Get the current device
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	// Load the constant buffer
	constantBuffer = LoadConstantBuffer(device, description.cBufferLayout);

	// Get the vertex shader
	vertexShader = LoadVertexShader(device, description.vShaderID);

	// Get the pixel shader
	pixelShader = LoadPixelShader(device, description.pShaderID);
};

Material::~Material(){
};
