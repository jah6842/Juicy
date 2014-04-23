#pragma once

#include "gameobject.h"
#include <vector>

class Enemy : public GameObject
{
public:
	Enemy(MESHES m, MATERIALS mat);
	~Enemy();
	void Update(float dt);
	void MoveEnemy(int speed);
	bool getActive();
private:
	std::vector<std::vector<Transform>> locations;
	bool active;
	int dimensions;
	int rowIndex;
	int columnIndex;
	float interval;
};

