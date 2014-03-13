#include "GameObject.h"

GameObject::GameObject(std::wstring meshName){
	// Register the GameObject with the renderer
	Renderer::RegisterGameObject(this);

	// Set the mesh
	mesh = Mesh::GetMesh(meshName);
};

GameObject::GameObject(){
	// Register the GameObject with the renderer
	Renderer::RegisterGameObject(this);

	// Set the material
	static int flip = 0;

	MATERIAL_DESCRIPTION matDesc;
	matDesc.cBufferLayout = CONSTANT_BUFFER_LAYOUT_PER_MODEL;
	matDesc.diffuseTexture = TM_TEXTURE_MARBLE;
	matDesc.textureFilter = TM_FILTER_POINT;
	matDesc.vShaderID = VSHADER_TEXTURED_LIT_INSTANCED;
	matDesc.pShaderID = PSHADER_TEXTURED_LIT_INSTANCED;
	matDesc.materialName = L"TexturedInstancedLighting";

	if(flip == 0){
		material = new Material(matDesc);
	} else if(flip == 1){
		matDesc.diffuseTexture = TM_TEXTURE_SAND;
		material =  new Material(matDesc);
	} else if(flip == 2){
		matDesc.diffuseTexture = TM_TEXTURE_SCALES;
		material =  new Material(matDesc);
	}
	
	flip++;
	if(flip > 2)
		flip = 0;
	
	// Set the mesh
	mesh = Mesh::GetMesh(L"StandardCube");
};

void GameObject::Update(float dt){
	//transform.Rotate(XMConvertToRadians(10.0f*dt), XMConvertToRadians(10.0f*dt), 0);
};

GameObject::~GameObject(){
	// Unregister this GameObject from the renderer
	Renderer::UnRegisterGameObject(this);
};
