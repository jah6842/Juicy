#ifndef _DEVICE_MANAGER_H
#define _DEVICE_MANAGER_H

#include <d3d11.h>
#include "Utilities.h"

// Static class to manage DirectX devices and contexts
// For our purposes, we'll probably only have one device and one context.

enum DEPTH_STENCIL_STATES {
	DM_STENCIL_ENABLE,
	DM_STENCIL_DISABLE,
	DM_STENCIL_SKYBOX
};

enum RASTERIZER_STATES {
	DM_CULL_NONE,
	DM_CULL_BACK,
	DM_WIREFRAME,
	DM_WIREFRAME_CULL_BACK
};

class DeviceManager {
private:
	static ID3D11Device* _currentDevice;
	static ID3D11DeviceContext* _currentDeviceContext;

	static ID3D11DepthStencilState* _dsEnableStencil;	// Text renderer uses this
	static ID3D11DepthStencilState* _dsDisableStencil;	// Default
	static ID3D11DepthStencilState* _dsLessEqual;		// Skybox uses this state

	static ID3D11RasterizerState* _rsCullBack;			// Cull back-facing polygons
	static ID3D11RasterizerState* _rsCullNone;			// Don't cull anything
	static ID3D11RasterizerState* _rsWireframe;			// Wireframe without culling
	static ID3D11RasterizerState* _rsWireframeCullBack; // Wireframe with back-face culling

public:

	// Initialize all of the device states
	static void Initialize(ID3D11Device* device);

	// Clean up states
	static void Cleanup(ID3D11Device* device);

	// Set the stencil state to use on the device
	static void SetStencilMode(ID3D11DeviceContext* deviceContext, DEPTH_STENCIL_STATES state);

	// Set the rasterizer state to use on the device
	static void SetRasterizerMode(ID3D11DeviceContext* deviceContext, RASTERIZER_STATES state);

	static ID3D11Device* GetCurrentDevice(){
		return _currentDevice;
	};
	static ID3D11DeviceContext* GetCurrentDeviceContext(){
		return _currentDeviceContext;
	};

	static void SetCurrentDevice(ID3D11Device* device){
		_currentDevice = device;
	};
	static void SetCurrentDeviceContext(ID3D11DeviceContext* deviceContext){
		_currentDeviceContext = deviceContext;
	};
};

#endif // _DEVICE_MANAGER_H