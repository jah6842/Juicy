#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "Utilities.h"

using namespace DirectX;

class Transform {
public:
	Transform();
	~Transform();

	// Update velocities
	void Update(float dt);

	// Getters
	XMFLOAT3 Pos();
	float PosX();
	float PosY();
	float PosZ();

	float RotX();
	float RotY();
	float RotZ();

	XMFLOAT3 Velocity();
	XMFLOAT3 RotationalVelocity();

	// Setters
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 pos);
	void SetRotation(float xRad, float yRad, float zRad);
	void SetRotation(XMFLOAT3 rotRad);
	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3 sca);
	void SetVelocity(float x, float y, float z);
	void SetVelocity(XMFLOAT3 vel);
	void SetRotationalVelocity(float x, float y, float z);
	void SetRotationalVelocity(XMFLOAT3 rotVel);

	// Velocity functions
	void AddVelocity(float x, float y, float z);
	void AddVelocity(XMFLOAT3 vel);
	void ClearVelocity();

	// Rotational Velocity functions
	void AddRotationalVelocity(float x, float y, float z);
	void AddRotationalVelocity(XMFLOAT3 rotVel);
	void ClearRotationalVelocity();

	// Movement functions
	void Move(float x, float y, float z);
	void Move(XMFLOAT3 pos);

	// Rotation functions
	void Rotate(float x, float y, float z);
	void Rotate(XMFLOAT3 rotRad);
	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);

	// Returns a model matrix of the pos, rot, and scale
	XMFLOAT4X4 WorldMatrix();
	void RecalcWorldMatrix();

	// Returns an XMVECTOR of the position, rotation, or scale. Unused 4th value is 0.0f.
	XMVECTOR PosToVector();
	XMVECTOR RotToVector();
	XMVECTOR ScaleToVector();

	// Returns an identity transform
	// Position/rotation 0,0,0, Scale 1,1,1
	static Transform Identity();

	// Returns a world matrix of the pos, rot, and scale
	static XMFLOAT4X4 WorldMatrix(Transform t);

private:
	// Internals
	XMFLOAT4X4 _matrix;
	XMFLOAT3 _position;
	XMFLOAT3 _rotation;
	XMFLOAT3 _scale;
	XMFLOAT3 _velocity;
	XMFLOAT3 _rotationalVelocity;
	bool _dirty;
};

#endif // _TRANSFORM_H