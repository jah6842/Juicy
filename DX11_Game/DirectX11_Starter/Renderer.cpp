#include "Renderer.h"

std::map<std::pair<MESHES, MATERIALS>, RenderBatch*> Renderer::_batches;
std::unordered_set<GameObject*> Renderer::_unbatchedObjects;

Renderer::Renderer(){
	_perFrameConstantBuffer = nullptr;
	_directionalLightBuffer = nullptr;
	_textMaterial = nullptr;
	textureManager = nullptr;
	rendererReady = false;
};

Renderer::~Renderer(){
	textureManager->Cleanup();
	ReleaseMacro(fontVBuffer);
};

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

	// Make sure we have things set up for text
	if(_textMaterial == nullptr){
		_textMaterial = Material::LoadMaterial(device, MATERIAL_TEXT);

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.CPUAccessFlags= D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.ByteWidth = sizeof(RenderVertex)*6*1024;
		HR(device->CreateBuffer(&vertexBufferDesc,NULL,&fontVBuffer));
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

	float ambientStrength = .1f;
	CONSTANT_BUFFER_DIRECTIONAL_LIGHT dirLight;
	dirLight.ambientColor = XMFLOAT4(ambientStrength,ambientStrength,ambientStrength,1.0f);
	dirLight.diffuseColor = XMFLOAT4(1,1,1,1);
	dirLight.lightDirection = XMFLOAT3(1.0f, 1.0f, 0.0f);

	deviceContext->UpdateSubresource(_directionalLightBuffer->cBuffer, 0, NULL, &dirLight, 0, 0);
	deviceContext->PSSetConstantBuffers(_directionalLightBuffer->slot,1,&_directionalLightBuffer->cBuffer);
};

ID3D11PixelShader* currentPixelShader = nullptr;
ID3D11VertexShader* currentVertexShader = nullptr;
ID3D11InputLayout* currentInputLayout = nullptr;
ID3D11Buffer* currentConstantBuffer = nullptr;
void Renderer::PrepareMaterial(GameObject* go, Material* mat){
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	// Check if we need to change state, if not use the current state.
	if(mat->vertexShader->vShader != currentVertexShader){
		currentVertexShader = mat->vertexShader->vShader;
		deviceContext->VSSetShader(mat->vertexShader->vShader, NULL, 0);
	}
	if(mat->vertexShader->vShaderInputLayout != currentInputLayout){
		currentInputLayout = mat->vertexShader->vShaderInputLayout;
		deviceContext->IASetInputLayout(mat->vertexShader->vShaderInputLayout);
	}
	if(mat->constantBuffer->cBuffer != currentConstantBuffer){
		deviceContext->VSSetConstantBuffers(1, 1, &mat->constantBuffer->cBuffer);
		currentConstantBuffer = mat->constantBuffer->cBuffer;
	}
	if(mat->pixelShader->pShader != currentPixelShader){
		currentPixelShader = mat->pixelShader->pShader;
		deviceContext->PSSetShader(mat->pixelShader->pShader, NULL, 0);
	}

	switch(mat->materialID){
		case MATERIAL_SKYBOX: 
			DeviceManager::SetStencilMode(deviceContext, DM_STENCIL_SKYBOX);
			DeviceManager::SetRasterizerMode(deviceContext, DM_CULL_NONE);
			break;
		default:
			DeviceManager::SetStencilMode(deviceContext, DM_STENCIL_ENABLE);
			DeviceManager::SetRasterizerMode(deviceContext, DM_CULL_BACK);
			break;
	}

	if(go == nullptr)
		return;

	switch(mat->constantBuffer->layout){
	case CONSTANT_BUFFER_LAYOUT_PER_MODEL:
		CONSTANT_BUFFER_PER_MODEL modelData;
		modelData.world = go->transform.WorldMatrix();

		// Update the constant buffer
		deviceContext->UpdateSubresource(mat->constantBuffer->cBuffer, 0, NULL, &modelData, 0, 0);
	}
};

void Renderer::Draw(){
	if(!rendererReady){
		PrepareRenderer();
	}

	currentPixelShader = nullptr;
	currentVertexShader = nullptr;
	currentInputLayout = nullptr;
	currentConstantBuffer = nullptr;

	UINT drawnObjects = 0;
	UINT drawCalls = 0;

	// Get the current device context
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();
	ID3D11Device* device = DeviceManager::GetCurrentDevice();

	//GameObject** renderList = new GameObject*[registeredGOs.size()];
	UINT renderCount = 0;

	std::vector<std::shared_ptr<Material>> materials;

	for(int i = 0; i < NUM_MATERIALS; i++){
		if(Material::allMaterials[i] != nullptr)
			materials.push_back(Material::allMaterials[i]);
	}
	/*
	for(std::unordered_set<GameObject*>::const_iterator itr = registeredGOs.begin(); itr != registeredGOs.end(); ++itr){
		// Check if the object is in the viewing frustum
		//if(!Camera::MainCamera.PointInFrustum((*itr)->transform.Pos()))
		//	continue;

		GameObject* go = *itr;
			
		// Check if the object is using an instanced material
		if(!go->material->vertexShader->isInstanced){
			PrepareMaterial(go, go->material.get());

			textureManager->SetActiveTexture(go->material->diffuseTexture);
			textureManager->SetActiveFilterMode(go->material->textureFilter);

			// Set the current vertex buffer
			UINT stride = Vertex::VertexSize(go->mesh->vertexType);
			UINT offset = 0;
			ID3D11Buffer* vBuffer = go->mesh->vertexBuffer;
			deviceContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
			// Set the current index buffer
			deviceContext->IASetIndexBuffer(go->mesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			// Set the topology
			deviceContext->IASetPrimitiveTopology(go->mesh->topology);
	
			// Draw individual model
			deviceContext->DrawIndexed(
				go->mesh->numIndices,	// The number of indices we're using in this draw
				0,
				0); 
			drawnObjects++;
		} 
	}

	std::shared_ptr<Material> currentRenderMaterial = nullptr;
	
	// Get all gameobjects with a certain material and draw them
	for(std::vector<std::shared_ptr<Material>>::const_iterator itr = materials.begin(); itr != materials.end(); itr++){

		// Get the next material
		currentRenderMaterial = (*itr);

		// Check if the material is not instanced, if so it was already rendered
		if(!currentRenderMaterial->vertexShader->isInstanced)
			continue;

		PrepareMaterial(nullptr, currentRenderMaterial.get());

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

		// Loop through all render items and put them into the instance array
		for (UINT i = 0; i < renderCount; i++) {
			//instances[i].modelMatrix = renderList[i]->transform.WorldMatrix();//XMLoadFloat4x4(&wMatrix);
		}

		// Do the drawing
		deviceContext->DrawIndexedInstanced(
			renderList[0]->mesh->numIndices,	// Index count per instance
			renderCount,						// # instances to render
			0,									// Start index location
			0,									// Start vertex location
			0);									// When to start using instances

		// Clean up
		currentRenderMaterial = nullptr;
		drawnObjects += renderCount;
		drawCalls++;
		renderCount = 0;
	}
	*/
	
	//delete[] renderList;
	rendererReady = false;
	
	std::string s1 = "Drawn Objects: " + std::to_string(drawnObjects);
	DrawString(s1.c_str(), 0, 100, 40);
	std::string s2 = "Draw calls: " + std::to_string(drawCalls);
	DrawString(s2.c_str(), 0, 140, 40);
	
};

void Renderer::DrawString(const char* text, float x, float y, float size, XMFLOAT4 color){
	
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	// Get screen width and height
	UINT width, height;
	Camera::MainCamera.GetScreenSize(width,height);

	float posX = (x - static_cast<float>(width)) / static_cast<float>(width);
	float posY = -(y+size - static_cast<float>(height)) / static_cast<float>(height);

    // size of one letter and string size
    int letterSize = sizeof(RenderVertex)*6;
    int textSize = strlen(text);

    // size of one char on screen
    float cScreenWidth = size / width;
    float cScreenHeight = size / height;

    // texel Size
    float texelWidth = 1.0f/16.0f;

	D3D11_MAPPED_SUBRESOURCE mappedSub;
	HR(deviceContext->Map(fontVBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedSub));

	// Pointer to buffer subresource
	RenderVertex* sprite = (RenderVertex*)mappedSub.pData;

	 //loop to build the string
    for(int i=0; i< textSize; i++)
    {
        /*Get starting position of the quad. First Quad is just the posX value , then characterwidth is added.*/
        float thisStartX = posX +(cScreenWidth * i);
        float thisEndX =thisStartX + cScreenWidth;
        float thisStartY = posY;
        float thisEndY = thisStartY + cScreenHeight;

        // Write the position of each 6 vertices to subresource
        sprite[0].pos = XMFLOAT3( thisEndX, thisEndY, 0.0f );
        sprite[1].pos = XMFLOAT3( thisEndX, thisStartY, 0.0f );
        sprite[2].pos = XMFLOAT3( thisStartX, thisStartY, 0.0f );
        sprite[3].pos = XMFLOAT3( thisStartX, thisStartY, 0.0f );
        sprite[4].pos = XMFLOAT3( thisStartX, thisEndY, 0.0f );
        sprite[5].pos = XMFLOAT3( thisEndX, thisEndY, 0.0f );

		// Get the row and column of the texture sheet
		int row = static_cast<int>(text[i]) / 16;
		int col = static_cast<int>(text[i]) % 16;
		
		// Get the texture coordinate based on row/col in texture sheet
		float texX = col / 16.0f;
		float texXL = texX + texelWidth;
		float texY = row / 16.0f;
		float texYL = texY + texelWidth;
		
        // Apply texture coordinates to subresource
        sprite[0].texCoord = XMFLOAT2( texXL, texY);
        sprite[1].texCoord = XMFLOAT2( texXL, texYL );
        sprite[2].texCoord = XMFLOAT2( texX, texYL );
        sprite[3].texCoord = XMFLOAT2( texX, texYL );
        sprite[4].texCoord = XMFLOAT2( texX, texY );
        sprite[5].texCoord = XMFLOAT2( texXL, texY );

		// Set the color attribute
		for(int j = 0; j < 6; j++){
			sprite[j].color = color;
		}

        //set sprite pointer for next sprite
        sprite = sprite + 6;
	}

	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Blend states
	/*
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	device->CreateBlendState(&blendDesc, &enableBlendingState);

	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	device->CreateBlendState(&blendDesc, &disableBlendingState);
	*/

	//deviceContext->OMSetBlendState(enableBlendingState, blendFactor, 0xFFFFFFFF);
	DeviceManager::SetStencilMode(deviceContext, DM_STENCIL_DISABLE);

	// Update the constant buffer itself
	CONSTANT_BUFFER_PER_MODEL modelData;
	modelData.world = Transform::Identity().WorldMatrix();
	deviceContext->UpdateSubresource(_textMaterial->constantBuffer->cBuffer, 0, NULL, &modelData, 0, 0);

	// Change shaders
	deviceContext->VSSetShader(_textMaterial->vertexShader->vShader, NULL, 0);
	deviceContext->IASetInputLayout(_textMaterial->vertexShader->vShaderInputLayout);
	deviceContext->VSSetConstantBuffers(1, 1, &_textMaterial->constantBuffer->cBuffer);
	deviceContext->PSSetShader(_textMaterial->pixelShader->pShader, NULL, 0);

	UINT stride = sizeof(RenderVertex);
	UINT offset = 0;
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetVertexBuffers(0,1,&fontVBuffer,&stride,&offset);

	textureManager->SetActiveTexture(_textMaterial->diffuseTexture);
	textureManager->SetActiveFilterMode(_textMaterial->textureFilter);

	deviceContext->Unmap(fontVBuffer, 0);
	deviceContext->Draw(6*textSize,0);

	//deviceContext->OMSetBlendState(disableBlendingState, blendFactor, 0xFFFFFFFF);
	DeviceManager::SetStencilMode(deviceContext, DM_STENCIL_ENABLE);

};

// Add a gameobject to the gameobjects list
void Renderer::RegisterGameObject(GameObject* go){

	if(go->material == nullptr || go->mesh == nullptr){
		return;
	}

	// If the material is not instanced, it shouldn't be added to a batch
	if(!go->material->vertexShader->isInstanced){
		_unbatchedObjects.insert(go);
		return;
	}

	std::pair<MESHES,MATERIALS> object = std::make_pair(go->mesh->meshID, go->material->materialID);

	// Check if a batch has been made for this pair yet
	if(!_batches.count(object)){
		_batches[object] = new RenderBatch(go->mesh, go->material);
	}

	// Add the gameobject to the batch
	_batches[object]->AddGameObject(go);
};

// Remove a gameobject from the gameobjects list
void Renderer::UnRegisterGameObject(GameObject* go){

	if(go->material == nullptr || go->mesh == nullptr){
		return;
	}

	if(!go->material->vertexShader->isInstanced){
		std::unordered_set<GameObject*>::iterator itr;
		itr = _unbatchedObjects.find(go);
		_unbatchedObjects.erase(itr);
		return;
	}

	std::pair<MESHES,MATERIALS> object = std::make_pair(go->mesh->meshID, go->material->materialID);
	_batches[object]->RemoveGameObject(go);
};
