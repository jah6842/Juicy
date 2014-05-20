#include "SpaceStation.h"


SpaceStation::SpaceStation(MESHES m, MATERIALS mat) : GameObject(m, mat)
{
	transform.SetScale(60,60,60);
	transform.SetPosition(40,-75,40);
}

SpaceStation::~SpaceStation() 
{
	// Unregister this GameObject from the renderer
	//Renderer::UnRegisterGameObject(this);
}

int SpaceStation::getHealth()
{
	return health;
}

void SpaceStation::setHealth(int hp)
{
	health = hp;
}

void SpaceStation::Update(float dt)
{
	
}

void SpaceStation::Collision()
{
	health -= 5;
}

bool SpaceStation::IsDestroyed()
{
	return (health <= 0);
}

void SpaceStation::Draw(Renderer* renderer)
{
	std::string healthString = "Cube Health: " + std::to_string(health);

	UINT screenWidth, screenHeight;
	Camera::MainCamera.GetScreenSize(screenWidth,screenHeight);

	float x = (500.0f / 800.0f) * (float)screenWidth;
	float y = (950.0f / 600.0f) * (float)screenHeight;

	renderer->DrawString(healthString.c_str(), x, y, 50.0f);
}
