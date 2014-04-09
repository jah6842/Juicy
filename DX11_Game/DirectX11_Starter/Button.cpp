#pragma once
#include "Button.h"


Button::Button(MESHES m, MATERIALS mat){
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	mesh = LoadMesh(device, m);


	// Set the material
	material = Material::LoadMaterial(device, mat);
};


Button::~Button(void)
{
	mesh = nullptr;
	material = nullptr;
};
