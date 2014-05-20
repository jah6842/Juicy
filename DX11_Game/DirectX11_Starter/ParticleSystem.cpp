#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(MESHES m, MATERIALS mat, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 accel, int max)
{
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	mesh = LoadMesh(device, m);
	material = Material::LoadMaterial(device, mat);

	position = pos; 
	velocity = vel;
	acceleration = accel;
	maxParticles = max;
	pPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	pVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	pAcceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	duration = 5;
	numParticles = 1;
	particles = (Particle**) malloc(sizeof(Particle*) * maxParticles);
	Particle* temp = new Particle(pPosition, pVelocity, pAcceleration, color, duration);
	particles[0] = temp;
	//Do not want it called in Draw method
	Renderer::UnRegisterGameObject(this);
}


ParticleSystem::~ParticleSystem(void)
{
	for(int i = 0; i < numParticles; i++)
	{
		delete particles[i];
	}
	delete particles;

	mesh = nullptr;
	material = nullptr;

	//Needs to be registered for GameObject Deallocator
	Renderer::RegisterGameObject(this);
}

void ParticleSystem::AddNewParticle(void)
{
	if(numParticles == maxParticles)
	{
		return;
	}
	Particle* temp = new Particle(pPosition, pVelocity, pAcceleration, color, duration);
	particles[numParticles] = temp;
	numParticles += 1;
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
		if(particles[i] == nullptr)
		{
			
		}
		if(particles[i]->IsDead())
		{
			RemoveParticle(i);
		}
		particles[i]->Update(dt);
	}
	if(numParticles < maxParticles)
	{
		AddNewParticle();
	}
}

void ParticleSystem::Draw(void)
{
	
}