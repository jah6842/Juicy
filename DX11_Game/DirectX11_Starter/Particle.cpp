#include "Particle.h"


Particle::Particle(XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 accel, XMFLOAT4 col, float dur)
{
	position = pos;
	velocity = vel;
	acceleration = accel;
	color = col;
	duration = dur;
}


Particle::~Particle(void)
{
}

void Particle::Update(float dt)
{
	velocity = XMFLOAT3(velocity.x + acceleration.x, velocity.y + acceleration.y, velocity.z + acceleration.z);
	position = XMFLOAT3(position.x + velocity.x, position.y + velocity.y, position.z + velocity.z);
	
	duration -= dt;

}

void Particle::Draw(void)
{

}

bool Particle::IsDead(void)
{
	if(duration <= 0)
	{
		return true;
	}
	return false;
}