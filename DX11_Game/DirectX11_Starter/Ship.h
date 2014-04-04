#pragma once

#include "gameobject.h"
#include <vector>

class Ship : public GameObject
{
public:
	Ship(MESHES m, MATERIALS mat);
	~Ship(void);
	void Update(float dt);
	void MoveShip(char direction);
private:
	std::vector<std::vector<Transform>> locations;
	int dimensions;
	int rowIndex;
	int columnIndex;
	float interval;
};

