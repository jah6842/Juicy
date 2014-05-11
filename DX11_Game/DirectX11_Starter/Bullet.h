#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(MESHES m, MATERIALS mat, int row, int column, FireMode type);
	~Bullet(void);

	bool CheckCollision();
	bool CheckOnScreen();
	void Update(float dt);
	int GetRow();
	int GetColumn();
	FireMode GetUpgradeType();
	void Collision();

private:
	//UPGRADE TYPE: 0 = NORMAL, 1= PIERCING, 2=WIDE, 3 = BURST
	FireMode upgradeType;
	bool isPiercing;
	int rowIndex;
	int columnIndex;
	bool collision;
};

