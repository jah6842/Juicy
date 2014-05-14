#pragma once

#include "gameobject.h"
#include <vector>

class SpaceStation : public GameObject
{
public:
	SpaceStation(MESHES m, MATERIALS mat);
	~SpaceStation();
	void Update(float dt);
	int getHealth();
	void setHealth(int health);
private:
	int health;
};
