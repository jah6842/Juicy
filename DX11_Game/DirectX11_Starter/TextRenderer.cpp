#include "TextRenderer.h"

ID3D11ShaderResourceView*	TextRenderer::fontTexture		= nullptr;
ID3D11SamplerState*			TextRenderer::fontTextureSampler= nullptr;
ID3D11Buffer*				TextRenderer::fontVBuffer		= nullptr;
ID3D11VertexShader*			TextRenderer::fontVShader		= nullptr;
ID3D11PixelShader*			TextRenderer::fontPShader		= nullptr;
ID3D11InputLayout*			TextRenderer::fontInputLayout	= nullptr;
ID3D11Buffer*				TextRenderer::fontCBuffer		= nullptr;
ID3D11BlendState*			TextRenderer::enableBlendingState = nullptr;
ID3D11BlendState*			TextRenderer::disableBlendingState= nullptr;
ID3D11DepthStencilState*	TextRenderer::disableStencil	= nullptr;
ID3D11DepthStencilState*	TextRenderer::enableStencil		= nullptr;
int							TextRenderer::maxStringLength	= 256;

void TextRenderer::Setup(){
	ID3D11Device* device = DeviceManager::GetCurrentDevice();
	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	// Load Texture
	std::wstring texPath = TEXTURE_PATH;
	texPath += L"ascii.png";
	DirectX::CreateWICTextureFromFile(device, deviceContext, texPath.c_str(), NULL, &fontTexture);

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    
	//Create the Sample State
	device->CreateSamplerState( &sampDesc, &fontTextureSampler );

	// Create vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags= D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.ByteWidth = sizeof(RenderVertex)*6*maxStringLength;
	HR(device->CreateBuffer(&vertexBufferDesc,NULL,&fontVBuffer));

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	std::wstring vShaderPath = SHADER_PATH;
	vShaderPath += L"FontVertex.cso";
	D3DReadFileToBlob(vShaderPath.c_str(), &vsBlob);

	// Create the shader on the device
	HR(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&fontVShader));

	//create the input layout
	D3D11_INPUT_ELEMENT_DESC* description = VERTEX_DESCRIPTION_NON_INSTANCED;
	UINT descriptionSize = 4;

	HR(device->CreateInputLayout(
		description,
		descriptionSize,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&fontInputLayout));

	ReleaseMacro(vsBlob);

	// Load Pixel Shader ---------------------------------------
	ID3DBlob* psBlob;
	std::wstring pShaderPath = SHADER_PATH;
	pShaderPath += L"FontPixel.cso";
	D3DReadFileToBlob(pShaderPath.c_str(), &psBlob);

	// Create the shader on the device
	HR(device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&fontPShader));

	// Clean up
	ReleaseMacro(psBlob);

	// Create a constant buffer
	D3D11_BUFFER_DESC cBufferDesc;

	cBufferDesc.ByteWidth = sizeof(CONSTANT_BUFFER_PER_MODEL);
	cBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags		= 0;
	cBufferDesc.MiscFlags			= 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(
		&cBufferDesc,
		NULL,
		&fontCBuffer));

	// Blend states
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

	

	// stencil states
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	device->CreateDepthStencilState(&depthStencilDesc, &enableStencil);

	// Clear the second depth stencil state before setting the parameters.
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	device->CreateDepthStencilState(&depthDisabledStencilDesc, &disableStencil);
};

void TextRenderer::Cleanup(){
	ReleaseMacro(fontTexture);
	ReleaseMacro(fontTextureSampler);
	ReleaseMacro(fontPShader);
	ReleaseMacro(fontVShader);
	ReleaseMacro(fontVBuffer);
	ReleaseMacro(fontInputLayout);
	ReleaseMacro(fontCBuffer);
	ReleaseMacro(enableBlendingState);
	ReleaseMacro(disableBlendingState);
	ReleaseMacro(enableStencil);
	ReleaseMacro(disableStencil);
};

void TextRenderer::DrawString(char* text, float x, float y, float size, XMFLOAT4 color){

	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	// Get screen width and height
	UINT width, height;
	Camera::MainCamera.GetScreenSize(width,height);

	float posX = (x - static_cast<float>(width)) / static_cast<float>(width);
	float posY = -(y+size - static_cast<float>(height)) / static_cast<float>(height);

    // size of one letter and string size
    int letterSize = sizeof(RenderVertex)*6;
    int textSize = strlen(text);

	// cut string if its to long
	if (textSize > maxStringLength){
        textSize = maxStringLength;
		LOG(L"STRING EXCEEDS MAX LENGTH");
	}

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
	deviceContext->OMSetBlendState(enableBlendingState, blendFactor, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(disableStencil, 1);

	// Update the constant buffer itself
	CONSTANT_BUFFER_PER_MODEL modelData;
	modelData.world = Transform::Identity().WorldMatrix();
	deviceContext->VSSetConstantBuffers(1, 1, &fontCBuffer);
	deviceContext->UpdateSubresource(fontCBuffer, 0, NULL, &modelData, 0, 0);

	deviceContext->VSSetShader(fontVShader,0,0);
	deviceContext->PSSetShader(fontPShader,0,0);

	UINT stride = sizeof(RenderVertex);
	UINT offset = 0;
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetVertexBuffers(0,1,&fontVBuffer,&stride,&offset);
	deviceContext->IASetInputLayout(fontInputLayout);
	deviceContext->PSSetShaderResources(0,1,&fontTexture);
	deviceContext->PSSetSamplers(0,1,&fontTextureSampler);

	deviceContext->Unmap(fontVBuffer, 0);
	deviceContext->Draw(6*textSize,0);

	deviceContext->OMSetBlendState(disableBlendingState, blendFactor, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(enableStencil, 1);
};