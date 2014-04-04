#ifndef _RENDER_BATCH_H
#define _RENDER_BATCH_H

// A render batch is a grouping of gameobjects that share the same
// material and mesh. At draw time they are batched into one draw
// call for efficiency.

#include <d3d11.h>
#include <DirectXMath.h>
#include <set>
#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
using namespace DirectX;

class GameObject;

struct PerInstanceData {
public:
	XMFLOAT4X4 modelMatrix;
};

class RenderBatch {
private:
	ID3D11Buffer*			_instanceBuffer;
	PerInstanceData*		_instances;
	UINT					_istanceBufferSize;
	D3D11_BUFFER_DESC		_instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA	_instanceData;

	std::unordered_set<GameObject*> _registeredGOs;
	std::shared_ptr<Material*> _material;
	std::shared_ptr<Mesh*> _mesh;

	void ResizeBatch(int newSize);

public:
	RenderBatch(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat);
	~RenderBatch();
	void AddGameObject(GameObject* go);
	void RemoveGameObject(GameObject* go);

	void PrepareBatch();
};

#endif // _RENDER_BATCH_H