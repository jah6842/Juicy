#pragma once


#include "GameObject.h"
#include "Particle.h"

using namespace DirectX;

class ParticleSystem : public GameObject
{
public:
	ParticleSystem(MESHES m, MATERIALS mat, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 accel, int max);
	~ParticleSystem(void);
	void AddNewParticle(void);
	void RemoveParticle(int index);
	void Update(float dt);
	void Draw(void);

	XMFLOAT3 position;
	XMFLOAT3 velocity;
	XMFLOAT3 acceleration;
	int maxParticles;
	XMFLOAT3 pPosition;
	XMFLOAT3 pVelocity;
	XMFLOAT3 pAcceleration;
	XMFLOAT4 color;
	float duration;
	

private:
	Particle** particles;
	int numParticles;
};

