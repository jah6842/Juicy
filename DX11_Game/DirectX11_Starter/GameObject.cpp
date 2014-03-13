#include "GameObject.h"

GameObject::GameObject(MESHES m, MATERIAL_DESCRIPTION mat){
	// Register the GameObject with the renderer
	Renderer::RegisterGameObject(this);

	// Set the mesh
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	mesh = LoadMesh(device, m);

	// Set the material
	material = new Material(mat);
};

GameObject::GameObject(){
	// Register the GameObject with the renderer
	Renderer::RegisterGameObject(this);
	mesh = nullptr;
	material = nullptr;
};

void GameObject::Update(float dt){
	//transform.Rotate(XMConvertToRadians(10.0f*dt), XMConvertToRadians(10.0f*dt), 0);
};

GameObject::~GameObject(){
	// Unregister this GameObject from the renderer
	Renderer::UnRegisterGameObject(this);
	// Free resources
	delete material;
};
