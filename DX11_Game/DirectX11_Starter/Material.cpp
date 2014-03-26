#include "Material.h"

std::map<UINT, std::shared_ptr<Material>> Material::allMaterials = std::map<UINT, std::shared_ptr<Material>>();

std::shared_ptr<Material> Material::LoadMaterial(ID3D11Device* device, MATERIALS mat){
	if(allMaterials[mat] != nullptr)
		return allMaterials[mat];

	std::shared_ptr<Material> m (new Material(mat));
	allMaterials[mat] = m;
	return allMaterials[mat];
};

// Constructor
Material::Material(MATERIALS mat){
	MATERIAL_DESCRIPTION description = MATERIAL_DESCRIPTIONS[mat];
	
	materialName = description.materialName;
	diffuseTexture = description.diffuseTexture;
	textureFilter = description.textureFilter;
	materialID = description.materialID;

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
