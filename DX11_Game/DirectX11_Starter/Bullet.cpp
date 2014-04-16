#include "Bullet.h"


Bullet::Bullet(MESHES m, MATERIALS mat, bool piercing) : GameObject(m, mat)
{
	isPiercing = piercing;
}


Bullet::~Bullet(void)
{
}

bool Bullet::CheckCollision(GameObject* object)
{
	if (isPiercing)
	{
		return false;
	}
}

bool Bullet::CheckOnScreen()
{
	return Camera::MainCamera.SphereInFrustum(transform.Pos(), 1.0f);
}

void Bullet::Update(float dt)
{
	transform.Update(dt);
}
