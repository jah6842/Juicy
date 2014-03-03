#ifndef _VERTEX_H
#define _VERTEX_H

#include <DirectXMath.h>
#include <d3d11.h>

using namespace DirectX;

enum VERTEX_TYPE
{
	VERTEX_TYPE_POS,
	VERTEX_TYPE_POS_UV,
	VERTEX_TYPE_POS_COLOR
};

struct InstanceType {
	XMFLOAT4X4 modelMatrix;
};

extern D3D11_INPUT_ELEMENT_DESC VERTEX_DESCRIPTION_POS_COLOR[];

extern D3D11_INPUT_ELEMENT_DESC VERTEX_DESCRIPTION_POS_UV[];

//http://www.rastertek.com/dx11tut37.html
extern D3D11_INPUT_ELEMENT_DESC VERTEX_DESCRIPTION_POS_UV_INSTANCED[];

// Vertex struct with position only
struct Vertex_POS
{
	Vertex_POS(){}
	Vertex_POS(float x, float y, float z) : pos(x,y,z){}
	Vertex_POS(XMFLOAT3 p) : pos(p){}

	XMFLOAT3 pos;
};

// Vertex struct with position and color
struct Vertex_POS_COLOR
{
	Vertex_POS_COLOR(){}
	Vertex_POS_COLOR(float x, float y, float z, XMFLOAT4 c) : pos(x,y,z), color(c){}
	Vertex_POS_COLOR(XMFLOAT3 p, XMFLOAT4 c) : pos(p), color(c){}

	XMFLOAT3 pos;
	XMFLOAT4 color;
};

// Vertex struct with position and UV
struct Vertex_POS_UV
{
	Vertex_POS_UV(){}
	Vertex_POS_UV(float x, float y, float z, float u, float v) : pos(x,y,z), texCoord(u, v){}
	Vertex_POS_UV(XMFLOAT3 p, float u, float v) : pos(p), texCoord(u, v){}
	Vertex_POS_UV(XMFLOAT3 p, XMFLOAT2 uv) : pos(p), texCoord(uv){}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
};

namespace Vertex {
	// Returns the size of each type of vertex struct
	static UINT VertexSize(VERTEX_TYPE type){
		UINT vertexSize;
		switch(type){
			case VERTEX_TYPE_POS: vertexSize = sizeof(Vertex_POS); break;
			case VERTEX_TYPE_POS_COLOR: vertexSize = sizeof(Vertex_POS_COLOR); break;
			case VERTEX_TYPE_POS_UV: vertexSize = sizeof(Vertex_POS_UV); break;
			default: vertexSize = 0; break;
		}

		assert(vertexSize != 0);
		return vertexSize;
	};

}

#endif // _VERTEX_H