#ifndef _VERTEX_H
#define _VERTEX_H

#include <DirectXMath.h>
#include <d3d11.h>

using namespace DirectX;

enum VERTEX_TYPE
{
	VERTEX_TYPE_NON_INSTANCED,
	VERTEX_TYPE_ALL,
	VERTEX_TYPE_2D
};

struct InstanceType {
	XMFLOAT4X4 modelMatrix;
};

extern D3D11_INPUT_ELEMENT_DESC VERTEX_DESCRIPTION_NON_INSTANCED[];
extern D3D11_INPUT_ELEMENT_DESC VERTEX_DESCRIPTION_ALL[];
extern D3D11_INPUT_ELEMENT_DESC VERTEX_DESCRIPTION_2D[];

struct RenderVertex 
{
	RenderVertex(){}
	RenderVertex(XMFLOAT3 p, XMFLOAT2 t, XMFLOAT3 n, XMFLOAT4 c) : pos(p), texCoord(t), normal(n), color(c){}
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	XMFLOAT4 color;
};

struct Render2DVertex
{
	Render2DVertex(){}
	Render2DVertex(XMFLOAT3 p, XMFLOAT2 t, XMFLOAT4 c) : pos(p), texCoord(t), color(c){}
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT4 color;
};

namespace Vertex {
	// Returns the size of each type of vertex struct
	static UINT VertexSize(VERTEX_TYPE type){
		UINT vertexSize;
		switch(type){
			case VERTEX_TYPE_ALL: vertexSize = sizeof(RenderVertex); break;
			case VERTEX_TYPE_2D: vertexSize = sizeof(Render2DVertex); break;
			default: vertexSize = 0; break;
		}

		assert(vertexSize != 0);
		return vertexSize;
	};
}

#endif // _VERTEX_H