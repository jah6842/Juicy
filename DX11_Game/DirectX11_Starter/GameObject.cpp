#include "GameObject.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0, 50);

GameObject::GameObject(std::wstring meshName){
	// Register the GameObject with the renderer
	Renderer::RegisterGameObject(this);

	// Set the material
	//material = Material::GetMaterial(L"TexturedInstanced", 1, L"texture.png");

	// Set the mesh
	mesh = Mesh::GetMesh(meshName);
};

GameObject::GameObject(){
	// Register the GameObject with the renderer
	Renderer::RegisterGameObject(this);

	// Set the material
	static int flip = 0;
	//if(flip == false)
		//material =  Material::GetMaterial(L"TexturedInstanced", 1, L"texture.png");
	MATERIAL_DESCRIPTION matDesc;
	matDesc.cBufferLayout = CONSTANT_BUFFER_LAYOUT_PER_MODEL;
	matDesc.diffuseTextureID = TEXTURE_MARBLE_PNG;
	matDesc.vShaderID = VSHADER_TEXTURED_LIT_INSTANCED;
	matDesc.pShaderID = PSHADER_TEXTURED_LIT_INSTANCED;
	matDesc.materialName = L"TexturedInstancedLighting";

	if(flip == 0){
		material = Material::GetMaterial(matDesc);
	} else if(flip == 1){
		matDesc.diffuseTextureID = TEXTURE_SAND_JPG;
		material =  Material::GetMaterial(matDesc);
	} else if(flip == 2){
		matDesc.diffuseTextureID = TEXTURE_SCALES_PNG;
		material =  Material::GetMaterial(matDesc);
	}
	
	flip++;
	if(flip > 2)
		flip = 0;
	
	// Set the mesh
	mesh = Mesh::GetMesh(L"StandardCube");
};

void GameObject::Update(float dt){
	transform.Rotate(XMConvertToRadians(10.0f*dt), XMConvertToRadians(10.0f*dt), 0);
};

GameObject::~GameObject(){
	// Unregister this GameObject from the renderer
	Renderer::UnRegisterGameObject(this);
};
