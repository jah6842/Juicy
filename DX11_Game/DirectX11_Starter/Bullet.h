#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(MESHES m, MATERIALS mat, int row, int column, bool piercing);
	~Bullet(void);

	bool CheckCollision();
	bool CheckOnScreen();
	void Update(float dt);
	int GetRow();
	int GetColumn();
	void Collision();

private:
	bool isPiercing;
	int rowIndex;
	int columnIndex;
	bool collision;
};

