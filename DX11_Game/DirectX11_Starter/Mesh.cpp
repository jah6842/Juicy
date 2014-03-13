#include "Mesh.h"

ID3D11Buffer* Mesh::CreateVertexBuffer(void* vertices, UINT numVertices, VERTEX_TYPE vertexType){
	// Get the curent Device
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	ID3D11Buffer* vBuffer;

	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
    vbd.Usage					= D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth				= Vertex::VertexSize(vertexType) * numVertices; // Number of vertices
    vbd.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags			= 0;
    vbd.MiscFlags				= 0;
	vbd.StructureByteStride		= 0;
    D3D11_SUBRESOURCE_DATA initialVertexData;
    initialVertexData.pSysMem	= vertices;
    HR(device->CreateBuffer(&vbd, &initialVertexData, &vBuffer));

	return vBuffer;
};

ID3D11Buffer* Mesh::CreateIndexBuffer(UINT* indices, UINT numIndices){
	// Get the curent Device
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	ID3D11Buffer* iBuffer;

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
    ibd.Usage					= D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth				= sizeof(UINT) * numIndices; // Number of indices
    ibd.BindFlags				= D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags			= 0;
    ibd.MiscFlags				= 0;
	ibd.StructureByteStride		= 0;
    D3D11_SUBRESOURCE_DATA initialIndexData;
    initialIndexData.pSysMem	= indices;
    HR(device->CreateBuffer(&ibd, &initialIndexData, &iBuffer));

	return iBuffer;
};
