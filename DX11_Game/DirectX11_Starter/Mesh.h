#ifndef _MESH_H
#define _MESH_H

#include <DirectXMath.h>
#include <d3d11.h>
#include "Vertex.h"
#include "Utilities.h"
#include <map>
#include <string>
#include "Constants.h"

using namespace DirectX;

struct Mesh {
	std::wstring name;
	MESHES meshID;
	VERTEX_TYPE vertexType;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	UINT numIndices;
	UINT numVertices;
	D3D_PRIMITIVE_TOPOLOGY topology;

	bool hasPosition;
	bool hasTexCoord;
	bool hasNormals;
	bool hasColor;
	bool is2D;

	Mesh(){
		name = L"UNINITIALIZED_MESH_NAME";
		vertexType = VERTEX_TYPE_ALL;
		indexBuffer = nullptr;
		vertexBuffer = nullptr;
		numIndices = 0;
		numVertices = 0;
		topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		is2D = false;
		hasColor = false;
		hasNormals = false;
		hasTexCoord = false;
		hasPosition = false;
	}
	~Mesh(){
		ReleaseMacro(vertexBuffer);
		ReleaseMacro(indexBuffer);
	}
	bool Is2D(){
		return is2D;
	}

	static ID3D11Buffer* CreateVertexBuffer(void* vertices, UINT numVertices, VERTEX_TYPE t);
	static ID3D11Buffer* CreateIndexBuffer(UINT* indices, UINT numIndices);
	static ID3D11Buffer* Create2DVertexBuffer(void* vertices, UINT numVertices, VERTEX_TYPE t);
};

////////// HARDCODED VERTICES //////////////
/* Until we get a model loader going :)   */
////////////////////////////////////////////
static RenderVertex StandardCubeVertices[24] =
{
	// Front Face
	// RenderVertex(Position, UV, Normal, Color)
	RenderVertex(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	// Back Face
	RenderVertex(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3( 1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f,  1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3( 1.0f,  1.0f, 1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3(-1.0f,  1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f,  1.0f, 1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	// Top Face
	RenderVertex(XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3(-1.0f, 1.0f,  1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 1.0f,  1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f, 1.0f,  1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f,  1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3( 1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	// Bottom Face
	RenderVertex(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	// Left Face
	RenderVertex(XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	// Right Face
	RenderVertex(XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
};
static UINT StandardCubeIndices[] = {
	// Front Face
	0,  1,  2,
	0,  2,  3,
	// Back Face
	4,  5,  6,
	4,  6,  7,
	// Top Face
	8,  9, 10,
	8, 10, 11,
	// Bottom Face
	12, 13, 14,
	12, 14, 15,
	// Left Face
	16, 17, 18,
	16, 18, 19,
	// Right Face
	20, 21, 22,
	20, 22, 23
};
static RenderVertex ButtonVertices[4] =
{
	RenderVertex(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f,  1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3( 1.0f,  1.0f, 0.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
	RenderVertex(XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT4(0.0f,0.0f,0.0f,0.0f)),
};
static UINT ButtonIndices[] = {
	0,  1,  2,
	2,  3,  0
};

#endif // _MESH_H