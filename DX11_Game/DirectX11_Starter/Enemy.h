#pragma once

#include "gameobject.h"
#include <vector>
#include "Bullet.h"

class Enemy : public GameObject
{
public:
	Enemy(MESHES m, MATERIALS mat, bool shooter);
	~Enemy();
	void Update(float dt);
	void MoveEnemy(int speed);
	bool getActive();
	void setActive(bool isActive);
	int GetRow();
	int GetColumn();
	void shoot();
private:
	std::vector<std::vector<Transform>> locations;
	std::vector<Bullet*> bullets;
	FireMode fireMode;
	bool active;
	bool canShoot;
	float shotCooldown;
	float regCooldown;
	float rapidCooldown;
	int dimensions;
	int rowIndex;
	int columnIndex;
	float interval;
};

