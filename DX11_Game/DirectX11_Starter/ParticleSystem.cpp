#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 accel, int max)
{
	position = pos; 
	velocity = vel;
	acceleration = accel;
	maxParticles = max;
	pPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	pVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	pAcceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	duration = 0;
	numParticles = 0;
	particles = (Particle**) malloc(sizeof(Particle*) * maxParticles);
}


ParticleSystem::~ParticleSystem(void)
{
	for(int i = 0; i < numParticles; i++)
	{
		delete particles[i];
	}
	delete particles;
}

void ParticleSystem::AddNewParticle(void)
{
	if(numParticles = maxParticles)
	{
		return;
	}
	Particle* temp = new Particle(pPosition, pVelocity, pAcceleration, color, duration);
	particles[numParticles] = temp;
	numParticles++;
}

void ParticleSystem::RemoveParticle(int index)
{
	if(particles[index] == NULL || index >= numParticles)
	{
		return;
	}
	delete particles[index];
	numParticles--;
	while(index != numParticles)
	{
		particles[index] = particles[index++];
	}
}

void ParticleSystem::Update(float dt)
{
	for(int i = 0; i < numParticles; i++)
	{
		particles[i]->Update(dt);
		if(particles[i]->IsDead())
		{
			RemoveParticle(i);
		}
	}
	if(numParticles < maxParticles)
	{
		AddNewParticle();
	}
}

void ParticleSystem::Draw(void)
{
	for(int i = 0; i < numParticles; i++)
	{
		particles[i]->Draw();
	}
}