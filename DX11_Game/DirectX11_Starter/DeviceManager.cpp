#include "DeviceManager.h"

ID3D11DepthStencilState* DeviceManager::_dsEnableStencil = nullptr;		// Text renderer uses this
ID3D11DepthStencilState* DeviceManager::_dsDisableStencil = nullptr;	// Default
ID3D11DepthStencilState* DeviceManager::_dsLessEqual = nullptr;			// Skybox uses this state

ID3D11RasterizerState* DeviceManager::_rsCullBack = nullptr;			// Cull back-facing polygons
ID3D11RasterizerState* DeviceManager::_rsCullNone = nullptr;			// Don't cull anything
ID3D11RasterizerState* DeviceManager::_rsWireframe = nullptr;			// Wireframe without culling
ID3D11RasterizerState* DeviceManager::_rsWireframeCullBack = nullptr;   // Wireframe with back-face culling

void DeviceManager::Initialize(ID3D11Device* device){

#pragma region STENCIL STATES
	// stencil states
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	device->CreateDepthStencilState(&depthStencilDesc, &_dsEnableStencil);

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthStencilDesc.DepthEnable = false;

	// Create the state using the device.
	device->CreateDepthStencilState(&depthStencilDesc, &_dsDisableStencil);

	// Re-zero the memory and set the comparison to LESS_EQUAL
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&depthStencilDesc, &_dsLessEqual);
#pragma endregion

#pragma region RASTERIZER STATES
	D3D11_RASTERIZER_DESC rasterDesc;
	// Initialize the description of the rasterizer state.
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));

	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthClipEnable = true;

	// DM_CULL_BACK
	device->CreateRasterizerState(&rasterDesc, &_rsCullBack);

	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

	// DM_WIREFRAME_CULL_BACK
	device->CreateRasterizerState(&rasterDesc, &_rsWireframeCullBack);

	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.CullMode = D3D11_CULL_NONE;

	// DM_WIREFRAME
	device->CreateRasterizerState(&rasterDesc, &_rsWireframe);

	rasterDesc.FillMode = D3D11_FILL_SOLID;

	// DM_CULL_NONE
	device->CreateRasterizerState(&rasterDesc, &_rsCullNone);

#pragma endregion

};

// Clean up states
void DeviceManager::Cleanup(ID3D11Device* device){
	ReleaseMacro(_dsEnableStencil);
	ReleaseMacro(_dsDisableStencil);
	ReleaseMacro(_dsLessEqual);

	ReleaseMacro(_rsCullBack);
	ReleaseMacro(_rsCullNone);
	ReleaseMacro(_rsWireframe);
	ReleaseMacro(_rsWireframeCullBack);
};

// Set the stencil state to use on the device
void DeviceManager::SetStencilMode(ID3D11DeviceContext* deviceContext, DEPTH_STENCIL_STATES state){
	switch(state){ 
		case DM_STENCIL_ENABLE: deviceContext->OMSetDepthStencilState(_dsEnableStencil, 1); break;
		case DM_STENCIL_DISABLE: deviceContext->OMSetDepthStencilState(_dsDisableStencil, 1); break;
		case DM_STENCIL_SKYBOX: deviceContext->OMSetDepthStencilState(_dsLessEqual, 1); break;
	}
};

// Set the rasterizer state to use on the device
void DeviceManager::SetRasterizerMode(ID3D11DeviceContext* deviceContext, RASTERIZER_STATES state){
	switch(state){
		case DM_CULL_NONE: deviceContext->RSSetState(_rsCullNone); break;
		case DM_CULL_BACK: deviceContext->RSSetState(_rsCullBack); break;
		case DM_WIREFRAME: deviceContext->RSSetState(_rsWireframe); break;
		case DM_WIREFRAME_CULL_BACK: deviceContext->RSSetState(_rsWireframeCullBack); break;
	}
};