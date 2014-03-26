#include "Skybox.h"

Skybox::Skybox(MATERIALS mat) : GameObject(MESH_SPHERE, mat){
	transform.SetScale(5.0f, 5.0f, 5.0f);
	
};

Skybox::~Skybox(){

};

void Skybox::Update(float dt){
	// Always keep the skybox centered on the camera position
	transform.SetPosition( Camera::MainCamera.GetPosition() );
};