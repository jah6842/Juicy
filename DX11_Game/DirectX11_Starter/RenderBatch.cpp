#include "RenderBatch.h"

RenderBatch::RenderBatch(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat){
	_mesh = mesh;
	_material = mat;

	// Allocate memory for all of the instance data
	_instances = new PerInstanceData[1000];

	ResizeBatch(_registeredGOs.size());
};

RenderBatch::~RenderBatch(){
	// Clean up
	ReleaseMacro(_instanceBuffer);
	delete[] _instances;
};

void RenderBatch::ResizeBatch(int newSize){
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	ReleaseMacro(_instanceBuffer);

	// Set up the description of the instance buffer.
	_instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	_instanceBufferDesc.ByteWidth = sizeof(PerInstanceData) * _registeredGOs.size();
	_instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	_instanceBufferDesc.CPUAccessFlags = 0;
	_instanceBufferDesc.MiscFlags = 0;
	_instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	_instanceData.pSysMem = _instances;
	_instanceData.SysMemPitch = 0;
	_instanceData.SysMemSlicePitch = 0;

	// Create an instance buffer for the instance data
	HR(device->CreateBuffer(&_instanceBufferDesc, &_instanceData, &_instanceBuffer));
};

void RenderBatch::PrepareBatch(){
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();
	
	// Set buffers in the input assembler
	UINT strides[2];
	UINT offsets[2];
	ID3D11Buffer* bufferPointers[2];

	//strides[0] = Vertex::VertexSize();
	strides[1] = sizeof(PerInstanceData);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = _mesh->vertexBuffer; 
	bufferPointers[1] = _instanceBuffer;

	// Set the current vertex buffer
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	// Set the current index buffer
	deviceContext->IASetIndexBuffer(_mesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the topology
	deviceContext->IASetPrimitiveTopology(_mesh->topology);
};

void RenderBatch::AddGameObject(GameObject* go){
	_registeredGOs.insert(go);
};

void RenderBatch::RemoveGameObject(GameObject* go){
	std::unordered_set<GameObject*>::iterator itr;
	itr = _registeredGOs.find(go);
	_registeredGOs.erase(itr);
};