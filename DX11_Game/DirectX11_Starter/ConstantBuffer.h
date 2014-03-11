#ifndef _CONSTANT_BUFFER_H
#define _CONSTANT_BUFFER_H

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

// CONSTANT BUFFERS MUST BE IN MULTIPLES OF 16 BYTES!
// CONSTANT BUFFERS MUST BE IN MULTIPLES OF 16 BYTES!
// Add padding where necessary.
// Did I mention?: CONSTANT BUFFERS MUST BE IN MULTIPLES OF 16 BYTES!

enum CONSTANT_BUFFER_LAYOUT {
	CONSTANT_BUFFER_LAYOUT_PER_FRAME,
	CONSTANT_BUFFER_LAYOUT_PER_MODEL,
	CONSTANT_BUFFER_LAYOUT_DIRECTIONAL_LIGHT
};

// CBuffer slot 0
struct CONSTANT_BUFFER_PER_FRAME {
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT4X4 viewProj;
};

// CBuffer slot 1
struct CONSTANT_BUFFER_PER_MODEL {
	XMFLOAT4X4 world;
};

// CBuffer slot 2
struct CONSTANT_BUFFER_DIRECTIONAL_LIGHT {
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;
	float padding;
};

#endif // _CONSTANT_BUFFER_H