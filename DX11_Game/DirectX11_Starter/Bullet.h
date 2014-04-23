#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(MESHES m, MATERIALS mat, bool piercing);
	~Bullet(void);

	bool CheckCollision(GameObject* object);
	bool CheckOnScreen();
	void Update(float dt);

private:
	bool isPiercing;
	// target?
};

