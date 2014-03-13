#include "Transform.h"

Transform::Transform(){
	_position = XMFLOAT3(0,0,0);
	_rotation = XMFLOAT3(0,0,0);
	_scale    = XMFLOAT3(1,1,1);
	_velocity = XMFLOAT3(0,0,0);
	_rotationalVelocity = XMFLOAT3(0,0,0);
	_dirty = true;
};

Transform::~Transform(){

};

// Update velocities
void Transform::Update(float dt){
	Move(_velocity.x * dt,_velocity.y * dt,_velocity.z * dt);
	Rotate(_rotationalVelocity.x * dt, _rotationalVelocity.y * dt, _rotationalVelocity.z * dt);
};

// Getters
XMFLOAT3 Transform::Pos(){
	return _position;
};
float Transform::PosX(){
	return _position.x;
};
float Transform::PosY(){
	return _position.y;
};
float Transform::PosZ(){
	return _position.z;
};

XMFLOAT3 Transform::Velocity(){
	return _velocity;
};

XMFLOAT3 Transform::RotationalVelocity(){
	return _rotationalVelocity;
};

// Setters
void Transform::SetPosition(float x, float y, float z){
	_position = XMFLOAT3(x,y,z);
	_matrix._14 = x;
	_matrix._24 = y;
	_matrix._34 = z;
};
void Transform::SetPosition(XMFLOAT3 pos){
	_position = pos;
	_matrix._14 = pos.x;
	_matrix._24 = pos.y;
	_matrix._34 = pos.z;
};
void Transform::SetRotation(float xRad, float yRad, float zRad){
	_rotation = XMFLOAT3(xRad, yRad, zRad);
	_dirty = true;
};
void Transform::SetRotation(XMFLOAT3 rotRad){
	_rotation = rotRad;
	_dirty = true;
};
void Transform::SetScale(float x, float y, float z){
	_scale = XMFLOAT3(x,y,z);
	_dirty = true;
};
void Transform::SetScale(XMFLOAT3 sca){
	_scale = sca;
	_dirty = true;
};
void Transform::SetVelocity(float x, float y, float z){
	_velocity = XMFLOAT3(x,y,z);
};
void Transform::SetVelocity(XMFLOAT3 vel){
	SetVelocity(vel.x, vel.y, vel.z);
};
void Transform::SetRotationalVelocity(float x, float y, float z){
	_rotationalVelocity.x = x;
	_rotationalVelocity.y = y;
	_rotationalVelocity.z = z;
};
void Transform::SetRotationalVelocity(XMFLOAT3 rotVel){
	SetRotationalVelocity(rotVel.x, rotVel.y, rotVel.z);
};

// Velocity functions
void Transform::AddVelocity(float x, float y, float z){
	_velocity.x += x;
	_velocity.y += y;
	_velocity.z += z;
};
void Transform::AddVelocity(XMFLOAT3 vel){
	AddVelocity(vel.x, vel.y, vel.z);
};
void Transform::ClearVelocity(){
	_velocity = XMFLOAT3(0,0,0);
};

// Rotational Velocity functions
void Transform::AddRotationalVelocity(float x, float y, float z){
	_rotationalVelocity.x += x;
	_rotationalVelocity.y += y;
	_rotationalVelocity.z += z;
};
void Transform::AddRotationalVelocity(XMFLOAT3 rotVel){
	AddRotationalVelocity(rotVel.x, rotVel.y, rotVel.z);
};
void Transform::ClearRotationalVelocity(){
	_rotationalVelocity = XMFLOAT3(0,0,0);
};

// Movement functions
void Transform::Move(float x, float y, float z){
	_position.x += x;
	_position.y += y;
	_position.z += z;
	_matrix._14 += x;
	_matrix._24 += y;
	_matrix._34 += z;
};
void Transform::Move(XMFLOAT3 pos){
	Move(pos.x, pos.y, pos.z);
};

// Rotation functions
void Transform::Rotate(float xRad, float yRad, float zRad){
	if(xRad != 0.0f)
		RotateX(xRad);
	if(yRad != 0.0f)
		RotateY(yRad);
	if(zRad != 0.0f)
		RotateZ(zRad);
};
void Transform::Rotate(XMFLOAT3 rotRad){
	Rotate(rotRad.x, rotRad.y, rotRad.z);
};
void Transform::RotateX(float xRad){
	_rotation.x += xRad;
	_dirty = true;
};
void Transform::RotateY(float yRad){
	_rotation.y += yRad;
	_dirty = true;
};
void Transform::RotateZ(float zRad){
	_rotation.z += zRad;
	_dirty = true;
};

// Returns a model matrix of the pos, rot, and scale
void Transform::RecalcWorldMatrix(){
	XMMATRIX pos = XMMatrixTranslation(_position.x, _position.y, _position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z);
	XMMATRIX sca = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	
	XMMATRIX model = sca * rot * pos;

	XMStoreFloat4x4(&_matrix, XMMatrixTranspose(model));
};

// Returns a model matrix of the pos, rot, and scale
XMFLOAT4X4 Transform::WorldMatrix(){
	if(_dirty){
		RecalcWorldMatrix();
		_dirty = false;
	}
	return _matrix;
};

// Returns an XMVECTOR of the position, rotation, or scale. Unused 4th value is 0.0f.
XMVECTOR Transform::PosToVector(){
	XMVECTOR v = XMVectorSet(_position.x, _position.y, _position.z, 0.0f);
	return v;
};
XMVECTOR Transform::RotToVector(){
	XMVECTOR v = XMVectorSet(_rotation.x, _rotation.y, _rotation.z, 0.0f);
	return v;
};
XMVECTOR Transform::ScaleToVector(){
	XMVECTOR v = XMVectorSet(_scale.x, _scale.y, _scale.z, 0.0f);
	return v;
};

// Returns a model matrix of the pos, rot, and scale
XMFLOAT4X4 Transform::WorldMatrix(Transform t){
	XMMATRIX pos = XMMatrixTranslation(t._position.x, t._position.y, t._position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(t._rotation.x, t._rotation.y, t._rotation.z);
	XMMATRIX sca = XMMatrixScaling(t._scale.x, t._scale.y, t._scale.z);
	
	XMMATRIX model = sca * rot * pos;

	XMFLOAT4X4 modelMatrix;
	XMStoreFloat4x4(&modelMatrix, XMMatrixTranspose(model));
	return modelMatrix;
};

// Returns an identity transform
// Position/rotation 0,0,0, Scale 1,1,1
Transform Transform::Identity(){
	Transform t;
	t._position = XMFLOAT3(0,0,0);
	t._rotation = XMFLOAT3(0,0,0);
	t._scale    = XMFLOAT3(1,1,1);
	return t;
};