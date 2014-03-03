#ifndef _MESH_H
#define _MESH_H

#include <DirectXMath.h>
#include <d3d11.h>
#include "Vertex.h"
#include "Utilities.h"
#include <map>
#include <string>

using namespace DirectX;

class Mesh {
public:
	static std::map<std::wstring, Mesh*> meshes;

	static Mesh* GetMesh(std::wstring meshName);

	static void Cleanup();

	// Construct a mesh without vertices
	Mesh();

	// Construct a mesh by name
	Mesh(std::wstring meshName);
	
	// Construct a mesh with vertices, assume clockwise indices
	void ConstructMesh(std::wstring meshName, void* vertices, UINT numVertices, VERTEX_TYPE vertexType);

	// Construct a mesh with vertices, custom indices
	void ConstructMesh(std::wstring meshName, void* vertices, UINT numVertices, VERTEX_TYPE vertexType, UINT* indices, UINT numIndices);
	
	~Mesh();

	void CreateMeshFromModel(std::wstring modelName);

	void SetVertexBuffer(void* vertices, UINT numVertices, VERTEX_TYPE t);
	void SetIndexBuffer(UINT* indices, UINT numIndices);
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topo);

	UINT IndexCount();
	UINT VertexCount();
	VERTEX_TYPE VertexType();
	ID3D11Buffer* VertexBuffer();
	ID3D11Buffer* IndexBuffer();
	D3D_PRIMITIVE_TOPOLOGY Topology();

private:
	ID3D11Buffer* _vertexBuffer;
	UINT _numVertices;
	VERTEX_TYPE _vertexType;
	ID3D11Buffer* _indexBuffer;
	UINT _numIndices;

	// How is the data laid out? 
	D3D_PRIMITIVE_TOPOLOGY _topology; // default is D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST 
};

////////// HARDCODED VERTICES //////////////
/* Until we get a model loader going :)   */
////////////////////////////////////////////
static Vertex_POS_UV StandardCubeVertices[24] =
{
	// Front Face
	Vertex_POS_UV(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
	Vertex_POS_UV(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
	Vertex_POS_UV( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
	Vertex_POS_UV( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
	// Back Face
	Vertex_POS_UV(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
	Vertex_POS_UV( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
	Vertex_POS_UV( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
	Vertex_POS_UV(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),
	// Top Face
	Vertex_POS_UV(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
	Vertex_POS_UV(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
	Vertex_POS_UV( 1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
	Vertex_POS_UV( 1.0f, 1.0f, -1.0f, 1.0f, 1.0f),
	// Bottom Face
	Vertex_POS_UV(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
	Vertex_POS_UV( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
	Vertex_POS_UV( 1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
	Vertex_POS_UV(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),
	// Left Face
	Vertex_POS_UV(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
	Vertex_POS_UV(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
	Vertex_POS_UV(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
	Vertex_POS_UV(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
	// Right Face
	Vertex_POS_UV( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
	Vertex_POS_UV( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
	Vertex_POS_UV( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
	Vertex_POS_UV( 1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
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

static XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
static XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
static XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

static Vertex_POS_COLOR StandardQuadVertices[4] = 
{
	Vertex_POS_COLOR( XMFLOAT3(-1.0f, +1.0f, +0.0f), red),
	Vertex_POS_COLOR( XMFLOAT3(+1.0f, -1.0f, +0.0f), red),
	Vertex_POS_COLOR( XMFLOAT3(-1.0f, -1.0f, +0.0f), blue),
	Vertex_POS_COLOR( XMFLOAT3(+1.0f, +1.0f, +0.0f), blue)
};
static UINT StandardQuadIndices[6] = { 0, 1, 2, 1, 0, 3 };

#endif // _MESH_H