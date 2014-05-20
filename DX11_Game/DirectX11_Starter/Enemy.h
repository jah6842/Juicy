#pragma once

#include "gameobject.h"
#include <vector>
#include "Bullet.h"

class Enemy : public GameObject
{
public:
	Enemy(MESHES m, MATERIALS mat, FireMode fire, bool shooter);
	~Enemy();
	void Update(float dt);
	void MoveEnemy(int speed);
	bool getActive();
	void setActive(bool isActive);
	int GetRow();
	int GetColumn();
	void shoot();
	std::vector<Bullet*> GetBullets();
private:
	std::vector<std::vector<Transform>> locations;
	//bullet stuff
	std::vector<Bullet*> bullets;
	FireMode fireMode;
	bool canShoot;
	float shotCooldown;
	float regCooldown;
	float rapidCooldown;
	//for rapid fire
	int burst;

	bool active;
	int dimensions;
	int rowIndex;
	int columnIndex;
	float interval;
	GameObject* marker;
};

