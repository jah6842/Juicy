#include "SpaceStation.h"


SpaceStation::SpaceStation(MESHES m, MATERIALS mat) : GameObject(m, mat)
{
	transform.SetPosition(0,-200, 0);
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
