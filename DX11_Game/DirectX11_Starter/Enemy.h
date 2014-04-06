#pragma once

#include "gameobject.h"
#include <vector>

class Enemy : public GameObject
{
public:
	Enemy(MESHES m, MATERIALS mat);
	~Enemy(void);
	void Update(float dt);
	void MoveEnemy(int speed);
private:
	std::vector<std::vector<Transform>> locations;
	int dimensions;
	int rowIndex;
	int columnIndex;
	float interval;
};

