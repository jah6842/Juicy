#ifndef _CAMERA_H
#define _CAMERA_H

// Camera class loosely based on
// http://takinginitiative.wordpress.com/2009/12/06/3d-directx10-free-look-camera-timer-based/

#include <DirectXMath.h>
#include <d3d11.h>
#include "Transform.h"

using namespace DirectX;

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {

public:
	// Create a camera
	Camera(UINT width,
		   UINT height,
		   float fov = 90.0f,
		   float nearClip = 0.1f,
		   float farClip = 1000.0f);
	// Destructor
	~Camera();

	// Update the camera once per frame
	// For now, the static main camera is the only one updated
	// If we have multiple later, it will be necessary to keep track of them
	// and update appropriately.
	void Update(float dt);
	
	// Getters
	XMFLOAT4X4 GetProjectionMatrix();
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetViewProjMatrix();
	void GetScreenSize(UINT& width, UINT& height);

	// Resize the camera and recalc necessary  matrices
	void Resize(UINT width, UINT height);

	// Returns true/false if a point is inside the camera's frustum
	bool PointInFrustum(float x, float y, float z);
	bool PointInFrustum(XMFLOAT3 position);

	/* TODO
	bool CubeInFrustum(float x, float y, float z, float radius);
	bool CubeInFrustum(XMFLOAT3 position, float radius);
	*/

	bool SphereInFrustum(float x, float y, float z, float radius);
	bool SphereInFrustum(XMFLOAT3 position, float radius);

	// A static camera
	static Camera MainCamera;

	void SetPosition(float x, float y, float z);
	void SetDirection(float heading, float pitch);
	void Rotate(float heading, float pitch);
	void Move(int i, float value);

private:
	// Projection matrix
	XMFLOAT4X4 _projectionMatrix;
	// View matrix
	XMFLOAT4X4 _viewMatrix;
	// Frustum matrix (6x4)
	float _frustum[6][4];

	// Point to look at
	XMFLOAT3 _dV, _dU;
	XMFLOAT3 _eye, _view, _up;
	XMFLOAT3 _forward, _right;
	float _heading, _pitch;
	float movementDirections[4];

	// Near and far clip plane distances
	float _nearClip;
	float _farClip;
	// Field of view (in radians)
	float _fov;
	// Width and heights in pixels
	UINT _width;
	UINT _height;

	// Recalculates appropriate matrices
	void RecalcProjMatrix();
	void RecalcViewMatrix();
	void RecalcFrustum();
};

#endif // _CAMERA_H