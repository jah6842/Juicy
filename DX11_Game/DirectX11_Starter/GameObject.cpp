#include "GameObject.h"

GameObject::GameObject(MESHES m, MATERIALS mat){
	// Set the mesh
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	mesh = LoadMesh(device, m);

	// Set the material
	material = Material::LoadMaterial(device, mat);

	// Register the GameObject with the renderer
	Renderer::RegisterGameObject(this);
};

GameObject::GameObject(){
	mesh = nullptr;
	material = nullptr;
	// Register the GameObject with the renderer
	Renderer::RegisterGameObject(this);
};

void GameObject::Update(float dt){
	transform.Update(dt);
};

GameObject::~GameObject(){
	// Unregister this GameObject from the renderer
	Renderer::UnRegisterGameObject(this);
};
