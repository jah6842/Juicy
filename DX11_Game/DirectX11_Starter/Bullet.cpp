#include "Bullet.h"


Bullet::Bullet(MESHES m, MATERIALS mat, int row, int column, bool piercing) : GameObject(m, mat)
{
	rowIndex = row;
	columnIndex = column;
	isPiercing = piercing;
	collision = false;
}


Bullet::~Bullet(void)
{
}

bool Bullet::CheckCollision()
{
	if (isPiercing)
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

int Bullet::GetColumn()
{
	return columnIndex;
}

void Bullet::Collision()
{
	collision = true;
}
