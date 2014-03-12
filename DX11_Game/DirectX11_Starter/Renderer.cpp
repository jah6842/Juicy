#include "Renderer.h"

std::unordered_set<GameObject*> Renderer::registeredGOs = std::unordered_set<GameObject*>();
std::shared_ptr<ConstantBuffer> Renderer::_perFrameConstantBuffer = nullptr;
std::shared_ptr<ConstantBuffer> Renderer::_directionalLightBuffer = nullptr;
TextureManager* Renderer::textureManager = nullptr;
bool Renderer::rendererReady = false;

Renderer::Renderer(){

};

Renderer::~Renderer(){

};

ID3D11Buffer* instanceBuffer = nullptr;
InstanceType* instances;
UINT instanceBufferSize;
D3D11_BUFFER_DESC instanceBufferDesc;
D3D11_SUBRESOURCE_DATA instanceData;

void Renderer::PrepareRenderer() {
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();
	
	// Check if the texture manager has been created
	if(textureManager == nullptr){
		textureManager = new TextureManager();
		textureManager->Init();
	}

	// Create a constant buffer for per-frame data
	if( _perFrameConstantBuffer == nullptr) {
		_perFrameConstantBuffer = LoadConstantBuffer(device, CONSTANT_BUFFER_LAYOUT_PER_FRAME);
	}

	// Set the constant buffer's data
	CONSTANT_BUFFER_PER_FRAME perFrameData;
	perFrameData.proj = Camera::MainCamera.GetProjectionMatrix();
	perFrameData.view = Camera::MainCamera.GetViewMatrix();
	perFrameData.viewProj = Camera::MainCamera.GetViewProjMatrix();
	deviceContext->UpdateSubresource(_perFrameConstantBuffer->cBuffer, 0, NULL, &perFrameData, 0, 0);
	deviceContext->VSSetConstantBuffers(_perFrameConstantBuffer->slot,1,&_perFrameConstantBuffer->cBuffer);

	PrepareLighting();

	rendererReady = true;
};

void Renderer::PrepareLighting(){
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	// Set the directional light data
	if( _directionalLightBuffer == nullptr) {
		_directionalLightBuffer = LoadConstantBuffer(device, CONSTANT_BUFFER_LAYOUT_DIRECTIONAL_LIGHT);
	}

	CONSTANT_BUFFER_DIRECTIONAL_LIGHT dirLight;
	dirLight.ambientColor = XMFLOAT4(.1f,.1f,.1f,.1f);
	dirLight.diffuseColor = XMFLOAT4(1,1,1,1);
	dirLight.lightDirection = XMFLOAT3(1.0f, 1.0f, 0.0f);

	deviceContext->UpdateSubresource(_directionalLightBuffer->cBuffer, 0, NULL, &dirLight, 0, 0);
	deviceContext->PSSetConstantBuffers(_directionalLightBuffer->slot,1,&_directionalLightBuffer->cBuffer);
};

void Renderer::Draw(){
	if(!rendererReady){
		PrepareRenderer();
	}

	ID3D11PixelShader* currentPixelShader = nullptr;
	ID3D11VertexShader* currentVertexShader = nullptr;
	ID3D11InputLayout* currentInputLayout = nullptr;
	ID3D11Buffer* currentConstantBuffer = nullptr;

	UINT drawnObjects = 0;
	UINT drawCalls = 0;

	// Get the current device context
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	GameObject** renderList = new GameObject*[registeredGOs.size()];
	UINT renderCount = 0;

	std::list<Material*> _materials; 

	for(std::unordered_set<GameObject*>::iterator itr = registeredGOs.begin(); itr != registeredGOs.end(); ++itr){
		// Check if the object is in the viewing frustum
		if(!Camera::MainCamera.PointInFrustum((*itr)->transform.Pos()))
			continue;
			
		// Check if the object is using an instanced material
		if(!(*itr)->material->_vertexShader->isInstanced){
			(*itr)->material->SetConstantBufferData((*itr)->transform.WorldMatrix());

			// Set the current vertex buffer
			UINT stride = Vertex::VertexSize((*itr)->mesh->VertexType());
			UINT offset = 0;
			ID3D11Buffer* vBuffer = (*itr)->mesh->VertexBuffer();
			deviceContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
			// Set the current index buffer
			deviceContext->IASetIndexBuffer((*itr)->mesh->IndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
			// Set the topology
			deviceContext->IASetPrimitiveTopology((*itr)->mesh->Topology());
	
			// Draw individual model
			deviceContext->DrawIndexed(
				(*itr)->mesh->IndexCount(),	// The number of indices we're using in this draw
				0,
				0); 
			drawnObjects++;
		} else {
			_materials.push_back((*itr)->material);
		}
	}

	

	Material* currentRenderMaterial = nullptr;
	// Get all gameobjects with a certain material and draw them
	for(std::list<Material*>::iterator itr = _materials.begin(); itr != _materials.end(); itr++){

		// Check if the material is not instanced, if so it was already rendered
		if(!(*itr)->_vertexShader->isInstanced)
			continue;

		// Get the first material from the render list
		currentRenderMaterial = *itr;

		// Check if we need to change state, if not use the current state.
		if(currentRenderMaterial->_vertexShader->vShader != currentVertexShader){
			currentVertexShader = currentRenderMaterial->_vertexShader->vShader;
			deviceContext->VSSetShader(currentRenderMaterial->_vertexShader->vShader, NULL, 0);
		}
		if(currentRenderMaterial->_vertexShader->vShaderInputLayout != currentInputLayout){
			currentInputLayout = currentRenderMaterial->_vertexShader->vShaderInputLayout;
			deviceContext->IASetInputLayout(currentRenderMaterial->_vertexShader->vShaderInputLayout);
		}
		if(currentRenderMaterial->_constantBuffer->cBuffer != currentConstantBuffer){
			deviceContext->VSSetConstantBuffers(1, 1, &currentRenderMaterial->_constantBuffer->cBuffer);
			currentConstantBuffer = currentRenderMaterial->_constantBuffer->cBuffer;
		}
		if( currentRenderMaterial->_pixelShader->pShader != currentPixelShader){
			currentPixelShader =currentRenderMaterial->_pixelShader->pShader;
			deviceContext->PSSetShader(currentRenderMaterial->_pixelShader->pShader, NULL, 0);
		}

		currentRenderMaterial->SetConstantBufferData(Transform::Identity().WorldMatrix());

		textureManager->SetActiveTexture(currentRenderMaterial->diffuseTexture);
		textureManager->SetActiveFilterMode(currentRenderMaterial->textureFilter);

		// Loop through the open list and get all of the objects with the 
		// same material that we should add to the render list.
		for(std::unordered_set<GameObject*>::iterator go = registeredGOs.begin(); go != registeredGOs.end(); ++go){
			// Check if the object is in the viewing frustum
			if(!Camera::MainCamera.SphereInFrustum((*go)->transform.Pos(), 2.0f))
				continue;
			
			// Check if the object is using the current material
			if((*go)->material == currentRenderMaterial){
				renderList[renderCount] = *go;
				renderCount++;
			}
		}
		if(renderCount == 0)
			continue;

		// Allocate memory for all of the instance data
		instances = new InstanceType[renderCount];

		// Loop through all render items and put them into the instance array
		for (UINT i = 0; i < renderCount; i++) {
			instances[i].modelMatrix = renderList[i]->transform.WorldMatrix();//XMLoadFloat4x4(&wMatrix);
		}

		// Set up the description of the instance buffer.
		instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		instanceBufferDesc.ByteWidth = sizeof(InstanceType) * renderCount;
		instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instanceBufferDesc.CPUAccessFlags = 0;
		instanceBufferDesc.MiscFlags = 0;
		instanceBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the instance data.
		instanceData.pSysMem = instances;
		instanceData.SysMemPitch = 0;
		instanceData.SysMemSlicePitch = 0;

		// Create an instance buffer for the instance data
		HR(device->CreateBuffer(&instanceBufferDesc, &instanceData, &instanceBuffer));

		// Set buffers in the input assembler
		UINT strides[2];
		UINT offsets[2];
		ID3D11Buffer* bufferPointers[2];

		strides[0] = Vertex::VertexSize(renderList[0]->mesh->VertexType());
		strides[1] = sizeof(InstanceType);

		offsets[0] = 0;
		offsets[1] = 0;

		bufferPointers[0] = renderList[0]->mesh->VertexBuffer();	
		bufferPointers[1] = instanceBuffer;

		// Set the current vertex buffer
		deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
		// Set the current index buffer
		deviceContext->IASetIndexBuffer(renderList[0]->mesh->IndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		// Set the topology
		deviceContext->IASetPrimitiveTopology(renderList[0]->mesh->Topology());
	
		// Do the drawing
		deviceContext->DrawIndexedInstanced(
			renderList[0]->mesh->IndexCount(),	// Index count per instance
			renderCount,						// # instances to render
			0,									// Start index location
			0,									// Start vertex location
			0);									// When to start using instances

		// Clean up
		ReleaseMacro(instanceBuffer);
		delete[] instances;
		instances = nullptr;
		currentRenderMaterial = nullptr;
		drawnObjects += renderCount;
		drawCalls++;
		renderCount = 0;
	}

	delete[] renderList;
	rendererReady = false;
	//LOG(L"Rendered objects: ", std::to_wstring(drawnObjects), L" (Draw Calls: ", std::to_wstring(drawCalls), L")");
};

// Add a gameobject to the gameobjects list
void Renderer::RegisterGameObject(GameObject* go){
	registeredGOs.insert(go);
};

// Remove a gameobject from the gameobjects list
void Renderer::UnRegisterGameObject(GameObject* go){
	std::unordered_set<GameObject*>::iterator itr;
	itr = registeredGOs.find(go);
	registeredGOs.erase(itr);
};

// Clean up resources
void Renderer::Cleanup(){
	textureManager->Cleanup();
};