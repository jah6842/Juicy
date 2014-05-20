#include "Bullet.h"


Bullet::Bullet(MESHES m, MATERIALS mat, int row, int column, FireMode type) : GameObject(m, mat)
{
	rowIndex = row;
	columnIndex = column;
	upgradeType = type;
	collision = false;
}


Bullet::~Bullet(void)
{
}

bool Bullet::CheckCollision()
{
	if (upgradeType == 1)
	{
		return false;
	}

	return collision;
}

bool Bullet::CheckOnScreen()
{
	return Camera::MainCamera.SphereInFrustum(transform.Pos(), 1.0f);
}

void Bullet::Update(float dt)
{
	transform.Update(dt);
}

int Bullet::GetRow()
{
	return rowIndex;
}

FireMode Bullet::GetUpgradeType()
{
	return upgradeType;
}

int Bullet::GetColumn()
{
	return columnIndex;
}

void Bullet::Collision()
{
	collision = true;
}
