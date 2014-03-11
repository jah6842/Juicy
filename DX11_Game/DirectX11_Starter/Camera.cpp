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

	RecalcProjMatrix();
};

// Destructor
Camera::~Camera(){

};

// Update the camera once per frame
void Camera::Update(float dt){
	MainCamera.RecalcViewMatrix();
};

// Returns the camera's projection matrix
XMFLOAT4X4 Camera::GetProjectionMatrix(){
	return _projection;
};

// Returns the camera's view matrix
XMFLOAT4X4 Camera::GetViewMatrix(){
	return _view;
};

// Returns the camera's projection*view matrix
XMFLOAT4X4 Camera::GetViewProjMatrix(){
	XMMATRIX v = XMLoadFloat4x4(&_view);
	XMMATRIX p = XMLoadFloat4x4(&_projection);

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
	XMVECTOR position	= transform.PosToVector();
	XMVECTOR target		= XMVectorSet(0, 0, 1, 0);
	XMVECTOR up			= XMVectorSet(0, 1, 0, 0);
	XMMATRIX V			= XMMatrixLookToLH(position, target, up);
	XMStoreFloat4x4(&_view, XMMatrixTranspose(V));

	RecalcFrustum();
};

// Recalculate the viewing frustum
void Camera::RecalcFrustum(){

	XMMATRIX m_view = XMLoadFloat4x4(&_view);
	XMMATRIX m_projection = XMLoadFloat4x4(&_projection);
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
	XMStoreFloat4x4(&_projection, XMMatrixTranspose(P));
};

// Resize the camera
void Camera::Resize(UINT width, UINT height){
	_width = width;
	_height = height;
	RecalcProjMatrix();
};