#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
using namespace DirectX;

class Particle 
{
public:
	Particle(XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 accel, XMFLOAT4 col, float dur);
	~Particle(void);
	void Update(float dt);
	void Draw(void);
	bool IsDead(void);

private:
	XMFLOAT3 position;
	XMFLOAT3 velocity;
	XMFLOAT3 acceleration;
	XMFLOAT4 color;
	float duration;
	
};


