#include "Camera.h"

Camera Camera::MainCamera(0,0);

// Create a camera
Camera::Camera(UINT width,
			   UINT height,
			   float fov,
			   float nearClip,
			   float farClip ){
	_width = width;
	_height = height;
	_fov = XMConvertToRadians(fov);
	_nearClip = nearClip;
	_farClip = farClip;

	_dV		= XMFLOAT3(0,0,1);
	_dU		= XMFLOAT3(0,1,0);
	_eye		= XMFLOAT3(0,0,0);
	_view	= XMFLOAT3(0,0,1);
	_up		= XMFLOAT3(0,1,0);
	_forward = XMFLOAT3(0,0,1);
	_right	= XMFLOAT3(1,0,0);
	_heading = 0;
	_pitch = 0;

	//initialize movement toggles
    movementDirections[0] = 0;
    movementDirections[1] = 0;
    movementDirections[2] = 0;
    movementDirections[3] = 0;

	RecalcProjMatrix();
};

// Destructor
Camera::~Camera(){

};

// Extra math functions for XMFLOAT3
XMFLOAT3 operator*(XMFLOAT3 a, float b) {
    XMFLOAT3 temp = a;
	temp.x *= b;
	temp.y *= b;
	temp.z *= b;
	return temp;
};

XMFLOAT3 operator*(float b, XMFLOAT3 a) {
    XMFLOAT3 temp = a;
	temp.x *= b;
	temp.y *= b;
	temp.z *= b;
	return temp;
};

XMFLOAT3 operator+(XMFLOAT3 a, XMFLOAT3 b){
	XMFLOAT3 temp = a;
	temp.x += b.x;
	temp.y += b.y;
	temp.z += b.z;
	return temp;
};

// Update the camera once per frame
void Camera::Update(float dt){
	//update position - 1.5 unit per second
    _eye = _eye +  dt * ( movementDirections[0] + movementDirections[1] ) * 1.5f * _forward +
             dt * ( movementDirections[2] + movementDirections[3] ) * 1.5f * _right;

	ZeroMemory(&movementDirections, sizeof(float)*4);

	MainCamera.RecalcViewMatrix();
};

// Returns the camera's projection matrix
XMFLOAT4X4 Camera::GetProjectionMatrix(){
	return _projectionMatrix;
};

// Returns the camera's view matrix
XMFLOAT4X4 Camera::GetViewMatrix(){
	return _viewMatrix;
};

// Returns the camera's projection*view matrix
XMFLOAT4X4 Camera::GetViewProjMatrix(){
	XMMATRIX v = XMLoadFloat4x4(&_viewMatrix);
	XMMATRIX p = XMLoadFloat4x4(&_projectionMatrix);

	XMFLOAT4X4 viewProj;
	XMStoreFloat4x4(&viewProj, (p*v));
	return viewProj;
};

void Camera::GetScreenSize(UINT& width, UINT& height){
	width = _width;
	height = _height;
};

// Recalculate the view matrix
void Camera::RecalcViewMatrix(){
	// Set up view matrix (camera)

	//create rotation matrix
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(_pitch, _heading, 0.0f);

	XMVECTOR dVVec = XMVectorSet(_dV.x, _dV.y, _dV.z, 1.0f);
	XMVECTOR dUVec = XMVectorSet(_dU.x, _dU.y, _dU.z, 1.0f);

	//create new view and up vectors
	XMVECTOR viewVec = XMVector3TransformCoord(dVVec, rotMat);
	XMVECTOR upVec = XMVector3TransformCoord(dUVec, rotMat);

	XMVECTOR forwardVec = XMVector3Normalize(viewVec);
	XMVECTOR rightVec = XMVector3Cross(upVec, viewVec);
	rightVec = XMVector3Normalize(rightVec);

	XMStoreFloat3(&_forward, forwardVec);
	XMStoreFloat3(&_right, rightVec);

	XMVECTOR eyeVec = XMVectorSet(_eye.x, _eye.y, _eye.z, 1.0f);
	viewVec = XMVectorAdd(eyeVec, viewVec);

	XMStoreFloat3(&_view, viewVec);

	XMMATRIX V = XMMatrixLookAtLH(eyeVec, viewVec, upVec);
	XMStoreFloat4x4(&_viewMatrix, XMMatrixTranspose(V));

	RecalcFrustum();
};

void Camera::SetPosition(float x, float y, float z){
	_eye.x = x;
	_eye.y = y;
	_eye.z = z;
};
void Camera::SetDirection(float heading, float pitch){
	_heading = heading;
	_pitch = pitch;
};
void Camera::Rotate(float heading, float pitch){
	_heading += heading;
	_pitch += pitch;

	if(_pitch > XM_PI/2){
		_pitch = XM_PI/2;
	}
	if(_pitch < -XM_PI/2) {
		_pitch = -XM_PI/2;
	}
};
void Camera::Move(int i, float value){
	movementDirections[i] = value;
};

// To implement
//void LookAt(XMFLOAT3);
//void RotateTowards(XMFLOAT3);

// Recalculate the viewing frustum
void Camera::RecalcFrustum(){

	XMMATRIX m_view = XMLoadFloat4x4(&_viewMatrix);
	XMMATRIX m_projection = XMLoadFloat4x4(&_projectionMatrix);
	XMMATRIX viewProjection = XMMatrixMultiply(m_projection, m_view);

	XMFLOAT4X4 vp;// = _view * _projection;
	XMStoreFloat4x4(&vp, XMMatrixTranspose(viewProjection));

	// Left Frustum Plane
    // Add first column of the matrix to the fourth column
	_frustum[0][0] = vp._14 + vp._11; 
	_frustum[0][1] = vp._24 + vp._21;
	_frustum[0][2] = vp._34 + vp._31;
	_frustum[0][3] = vp._44 + vp._41;

	// Right Frustum Plane
    // Subtract first column of matrix from the fourth column
	_frustum[1][0] = vp._14 - vp._11; 
	_frustum[1][1] = vp._24 - vp._21;
	_frustum[1][2] = vp._34 - vp._31;
	_frustum[1][3] = vp._44 - vp._41;

	// Top Frustum Plane
    // Subtract second column of matrix from the fourth column
	_frustum[2][0] = vp._14 - vp._12; 
	_frustum[2][1] = vp._24 - vp._22;
	_frustum[2][2] = vp._34 - vp._32;
	_frustum[2][3] = vp._44 - vp._42;

	// Bottom Frustum Plane
    // Add second column of the matrix to the fourth column
	_frustum[3][0] = vp._14 + vp._12;
	_frustum[3][1] = vp._24 + vp._22;
	_frustum[3][2] = vp._34 + vp._32;
	_frustum[3][3] = vp._44 + vp._42;

	// Near Frustum Plane
    // We could add the third column to the fourth column to get the near plane,
    // but we don't have to do this because the third column IS the near plane
	_frustum[4][0] = vp._13;
	_frustum[4][1] = vp._23;
	_frustum[4][2] = vp._33;
	_frustum[4][3] = vp._43;

	// Far Frustum Plane
    // Subtract third column of matrix from the fourth column
	_frustum[5][0] = vp._14 - vp._13; 
	_frustum[5][1] = vp._24 - vp._23;
	_frustum[5][2] = vp._34 - vp._33;
	_frustum[5][3] = vp._44 - vp._43;

	for(int i = 0; i < 6; ++i)
	{
		float length = sqrt((_frustum[i][0] * _frustum[i][0]) + (_frustum[i][1] * _frustum[i][1]) + (_frustum[i][2] * _frustum[i][2]));
		_frustum[i][0] /= length;
		_frustum[i][1] /= length;
		_frustum[i][2] /= length;
		_frustum[i][3] /= length;
	}
};

// Returns true if a point lies within the camera's frustrum
bool Camera::PointInFrustum(float x, float y, float z){
	int p;
	for( p = 0; p < 6; p++ ){
		// Point is outside of frustum.
		if( _frustum[p][0] * x + _frustum[p][1] * y + _frustum[p][2] * z + _frustum[p][3]    <= 0 )
			return false;
	}
	return true;
};
bool Camera::PointInFrustum(XMFLOAT3 position){
	return PointInFrustum(position.x, position.y, position.z);
};

// Returns true if a sphere lies within the camera's frustrum
bool Camera::SphereInFrustum(float x, float y, float z, float radius){
	int p;
	for( p = 0; p < 6; p++ ){
		// Sphere is outside the frustum.
		if( _frustum[p][0] * x + _frustum[p][1] * y + _frustum[p][2] * z + _frustum[p][3]    <= -radius )
			return false;
	}
	return true;
};
bool Camera::SphereInFrustum(XMFLOAT3 position, float radius){
	return SphereInFrustum(position.x, position.y, position.z, radius);
};

// Recalculate the projection matrix
void Camera::RecalcProjMatrix(){
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		_fov,
		(float)_width / (float)_height,
		_nearClip,
		_farClip);
	XMStoreFloat4x4(&_projectionMatrix, XMMatrixTranspose(P));
};

// Resize the camera
void Camera::Resize(UINT width, UINT height){
	_width = width;
	_height = height;
	RecalcProjMatrix();
};