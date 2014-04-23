#pragma once

#include "gameobject.h"
#include <vector>
#include "KeyboardInput.h"
#include "Bullet.h"

class Ship : public GameObject
{
public:
	Ship(MESHES m, MATERIALS mat, KeyboardInput* kb);
	~Ship(void);
	void Update(float dt);
private:
	void Shoot();
	void Move();

	std::vector<std::vector<Transform>> locations;
	int dimensions;
	int rowIndex;
	int columnIndex;
	float interval;
	std::vector<Bullet*> bullets;
	KeyboardInput* keyboard;
	float shootCooldown;
	float shootTimer;
};

