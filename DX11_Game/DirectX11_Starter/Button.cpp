#pragma once
#include "Button.h"


Button::Button(MESHES m, MATERIALS mat, XMFLOAT4 dimensions){
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	mesh = LoadMesh(device, m);


	// Set the material
	material = Material::LoadMaterial(device, mat);
	x = dimensions.x;
	y = dimensions.y;
	width = dimensions.z;
	height = dimensions.w;

	UINT screenWidth, screenHeight;
	Camera::MainCamera.GetScreenSize(screenWidth,screenHeight);

	float scaleX = -1 + (2.0f * (x + (width / 2)) / static_cast<float>(screenWidth));
	float scaleY =  1 - (2.0f * ((y + (height / 2))  / static_cast<float>(screenHeight)));

	float widthScale = 2.0f * (width / static_cast<float>(screenWidth));
	float heightScale = 2.0f * (height / static_cast<float>(screenHeight));


	transform.SetPosition(scaleX,scaleY,0.0f);
	transform.SetScale(widthScale, heightScale, 0.0f);
	transform.Update(0);

	Visible = false;

	//Needs to unregister so it is not used in Draw() call
	Renderer::UnRegisterGameObject(this);
};


Button::~Button(void)
{
	mesh = nullptr;
	material = nullptr;
	//Needs to be registered for GameObject Deallocator
	Renderer::RegisterGameObject(this);
};

boolean Button::Clicked(POINT* p)
{
	//std::cout<< p->x <<std::endl;
	//std::cout<< x << " " << x + width << std::endl;
	//std::cout<< p->y <<std::endl;
	//std::cout<< y << " " << y + height << std::endl;
	if(p->x >= x && p->x <= x + width)
	{
		if(p->y >= y && p->y <= y + height)
		{
			return true;
		}
	}
	return false;
}