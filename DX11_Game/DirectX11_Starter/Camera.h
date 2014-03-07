#ifndef _CAMERA_H
#define _CAMERA_H

#include <DirectXMath.h>
#include <d3d11.h>
#include "Transform.h"

using namespace DirectX;

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
	static void Update(float dt);
	
	// Getters
	XMFLOAT4X4 GetProjectionMatrix();
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetViewProjMatrix();

	// Resize the camera and recalc necessary  matrices
	void Resize(UINT width, UINT height);

	// Returns true/false if a point is inside the camera's frustum
	bool PointInFrustum(float x, float y, float z);
	bool PointInFrustum(XMFLOAT3 position);

	// A static camera
	static Camera MainCamera;

	// The camera's transform is public, for simplicity
	Transform transform;
	
private:
	// Projection matrix
	XMFLOAT4X4 _projection;
	// View matrix
	XMFLOAT4X4 _view;
	// Frustum matrix (6x4)
	float _frustum[6][4];

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