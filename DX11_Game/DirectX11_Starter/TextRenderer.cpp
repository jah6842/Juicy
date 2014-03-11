#include "TextRenderer.h"

ID3D11ShaderResourceView*	TextRenderer::fontTexture		= nullptr;
ID3D11SamplerState*			TextRenderer::fontTextureSampler= nullptr;
ID3D11Buffer*				TextRenderer::fontVBuffer		= nullptr;
ID3D11VertexShader*			TextRenderer::fontVShader		= nullptr;
ID3D11PixelShader*			TextRenderer::fontPShader		= nullptr;
ID3D11InputLayout*			TextRenderer::fontInputLayout	= nullptr;
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
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
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
};

void TextRenderer::Cleanup(){
	ReleaseMacro(fontTexture);
	ReleaseMacro(fontTextureSampler);
	ReleaseMacro(fontPShader);
	ReleaseMacro(fontVShader);
	ReleaseMacro(fontVBuffer);
	ReleaseMacro(fontInputLayout);
};

void TextRenderer::DrawString(char* text, float x, float y, float size){

	ID3D11DeviceContext* deviceContext = DeviceManager::GetCurrentDeviceContext();

	float posX = x / 800.0f;
	float posY = y / 600.0f;

    // size of one letter and string size
    int letterSize = sizeof(RenderVertex)*6;
    int textSize = strlen(text);

	// cut string if its to long
	if (textSize > maxStringLength){
        textSize = maxStringLength;
		LOG(L"DISPLAYED STRING EXCEEDS MAX LENGTH");
	}

    // size of one char on screen
    float cScreenWidth = 1.0f;
    float cScreenHeight = 1.0f;

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
        float thisStartX = posX +(cScreenWidth * static_cast<char>(i));
        float thisEndX =thisStartX + cScreenWidth;
        float thisStartY = posY;
        float thisEndY = thisStartY + cScreenHeight;

        // Write the position of each 6 vertices to subresource
        sprite[0].pos = XMFLOAT3( thisEndX, thisEndY, 1.0f );
        sprite[1].pos = XMFLOAT3( thisEndX, posY, 1.0f );
        sprite[2].pos = XMFLOAT3( thisStartX, posY, 1.0f );
        sprite[3].pos = XMFLOAT3( thisStartX, posY, 1.0f );
        sprite[4].pos = XMFLOAT3( thisStartX, thisEndY, 1.0f );
        sprite[5].pos = XMFLOAT3( thisEndX, thisEndY, 1.0f );

        // calculate texture coordinates from ASCII values.
        //int texLookup = 0;
       // int letter = static_cast<int>(text[i]);

		int row = static_cast<int>(text[i]) / 16;
		int col = static_cast<int>(text[i]) % 16;

       // float texStart = 0.0f +( texelWidth * static_cast<float>(texLookup));
       // float texEnd = texStart + texelWidth;

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

        //set sprite pointer for next sprite
        sprite = sprite + 6;
	}

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
};