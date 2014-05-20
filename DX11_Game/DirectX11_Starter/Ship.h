#pragma once

#include "gameobject.h"
#include <vector>
#include "KeyboardInput.h"
#include "Bullet.h"
#include "fmod.hpp"
#include "fmod_errors.h"

class Ship : public GameObject
{
public:
	Ship(MESHES m, MATERIALS mat, KeyboardInput* kb, FMOD::System* systemFMOD);
	~Ship(void);
	void Update(float dt);
	void Draw(Renderer* renderer);
	vector<Bullet*> GetBullets();
	int GetRow();
	int GetColumn();
	void Collision(Bullet* bullet);
	bool IsDead();
private:
	void Shoot();
	void Move();
	void SoundErrorCheck(FMOD_RESULT result);

	FireMode fireMode;
	std::vector<std::vector<Transform>> locations;
	int dimensions;
	int rowIndex;
	int columnIndex;
	float interval;
	std::vector<Bullet*> bullets;
	KeyboardInput* keyboard;
	float shootCooldown;
	float shootTimer;
	float rapidCooldown;
	int lives;
	float rapidEnergy;
	float wideEnergy;
	float piercingEnergy;
	bool shield;
	bool powerup;
	float specialLength;

	FMOD_RESULT fmodResult;
	FMOD::System* fmodSystem;

	FMOD::Sound* laserSound;
	FMOD::Channel* laserChannel;
};

