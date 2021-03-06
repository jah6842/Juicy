#include "GameObject.h"

GameObject::GameObject(MESHES m, MATERIALS mat){
	// Register the GameObject with the renderer
	Renderer::RegisterGameObject(this);

	// Set the mesh
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	mesh = LoadMesh(device, m);

	// Set the material
	material = Material::LoadMaterial(device, mat);
};

GameObject::GameObject(){
	// Register the GameObject with the renderer
	Renderer::RegisterGameObject(this);
	mesh = nullptr;
	material = nullptr;
};

void GameObject::Update(float dt){
	transform.Update(dt);
};

GameObject::~GameObject(){
	// Unregister this GameObject from the renderer
	Renderer::UnRegisterGameObject(this);
};
