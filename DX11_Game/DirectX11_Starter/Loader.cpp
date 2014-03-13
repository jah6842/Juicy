#include "Loader.h"

// Returns a VertexShader struct for the desired shader
std::shared_ptr<VertexShader> LoadVertexShader(ID3D11Device* device, VSHADER vShaderID){

	if(vShaders[vShaderID] != nullptr){
		return vShaders[vShaderID];
	}

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	bool instanced = false;

	LOG(L"Creating vertex shader: ", std::to_wstring(vShaderID));

	// Load shader from file
	ID3DBlob* vsBlob;
	std::wstring vShaderPath = SHADER_PATH;
	vShaderPath += vShaderNames[vShaderID];
	D3DReadFileToBlob(vShaderPath.c_str(), &vsBlob);

	// Create the shader on the device
	HR(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&vertexShader));
	
	// Create an input description
	D3D11_INPUT_ELEMENT_DESC* description = nullptr;
	UINT descriptionSize = 0;

	switch(vShaderID){
	case VSHADER_TEXTURED_LIT_INSTANCED:
	case VSHADER_TEXTURED_INSTANCED:
	case VSHADER_COLORED:
		description = VERTEX_DESCRIPTION_ALL;
		descriptionSize = 8;
		instanced = true;
		break;
	default:
		LOG(L"UNSUPPORTED SHADER!");
		assert(description != nullptr);
		break;
	}

	// Before cleaning up the data, create the input layout
	HR(device->CreateInputLayout(
		description,
		descriptionSize,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout));

	// Clean up
	ReleaseMacro(vsBlob);

	std::shared_ptr<VertexShader> v (new VertexShader(vertexShader, inputLayout, vShaderID, instanced));
	vShaders[vShaderID] = v;

	return vShaders[vShaderID];
};

// Returns a pixel shader pointer for the desired shader
std::shared_ptr<PixelShader> LoadPixelShader(ID3D11Device* device, PSHADER pShaderID){

	if(pShaders[pShaderID] != nullptr){
		return pShaders[pShaderID];
	}

	LOG(L"Creating pixel shader: ", std::to_wstring(pShaderID));

	ID3D11PixelShader* pixelShader = nullptr;

	// Load shader from file
	ID3DBlob* psBlob;
	std::wstring pShaderPath = SHADER_PATH;
	pShaderPath += pShaderNames[pShaderID];
	D3DReadFileToBlob(pShaderPath.c_str(), &psBlob);

	// Create the shader on the device
	HR(device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&pixelShader));

	// Clean up
	ReleaseMacro(psBlob);

	std::shared_ptr<PixelShader> p (new PixelShader(pixelShader, pShaderID));
	pShaders[pShaderID] = p;

	return pShaders[pShaderID];
};

// Returns the desired constant buffer
std::shared_ptr<ConstantBuffer> LoadConstantBuffer(ID3D11Device* device, CBUFFER_LAYOUT layout){

	if(cBuffers[layout] != nullptr){
		return cBuffers[layout];
	}

	ID3D11Buffer* constantBuffer;
	UINT slot;

	// Create a constant buffer
	D3D11_BUFFER_DESC cBufferDesc;

	switch(layout){
	case CONSTANT_BUFFER_LAYOUT_PER_FRAME:
		cBufferDesc.ByteWidth = sizeof(CONSTANT_BUFFER_PER_FRAME);
		slot = 0;
		break;
	case CONSTANT_BUFFER_LAYOUT_PER_MODEL:
		cBufferDesc.ByteWidth = sizeof(CONSTANT_BUFFER_PER_MODEL);
		slot = 1;
		break;
	case CONSTANT_BUFFER_LAYOUT_DIRECTIONAL_LIGHT:
		cBufferDesc.ByteWidth = sizeof(CONSTANT_BUFFER_DIRECTIONAL_LIGHT);
		slot = 2;
		break;
	default:
		LOG(L"INVALID CONSTANT BUFFER TYPE! ");
	}

	cBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags		= 0;
	cBufferDesc.MiscFlags			= 0;
	cBufferDesc.StructureByteStride = 0;

	// Create the buffer
	HR(device->CreateBuffer( &cBufferDesc, NULL, &constantBuffer));

	std::shared_ptr<ConstantBuffer> c (new ConstantBuffer(constantBuffer, layout, slot));
	cBuffers[layout] = c;

	return cBuffers[layout];
};

// Returns a pointer to the desired mesh
std::shared_ptr<Mesh> LoadMesh(ID3D11Device* device, MESHES mesh){
	// Check if mesh already exists
	if(meshes[mesh] != nullptr){
		return meshes[mesh];
	}

	std::shared_ptr<Mesh> m (new Mesh());

	if(mesh == MESH_CUBE){
		m->hasColor = false;
		m->hasNormals = true;
		m->hasPosition = true;
		m->hasTexCoord = true;
		m->indexBuffer = Mesh::CreateIndexBuffer(StandardCubeIndices, 36);
		m->numIndices = 36;
		m->vertexBuffer = Mesh::CreateVertexBuffer(StandardCubeVertices, 24, VERTEX_TYPE_ALL);
		m->numVertices = 24;
		m->vertexType = VERTEX_TYPE_ALL;
		m->name = L"Cube";
		m->meshID = mesh;
		m->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	} else if (mesh == MESH_SHIP){
		m = LoadModel(L"frigate.fbx");
	}

	LOG(L"New mesh loaded: ", m->name);
	meshes[mesh] = m;

	return meshes[mesh];
};

std::shared_ptr<Mesh> LoadModel(std::wstring modelName){

	std::shared_ptr<Mesh> m (new Mesh());

	FbxManager* g_pFbxSdkManager = nullptr;

	if(g_pFbxSdkManager == nullptr)
	{
		g_pFbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT );
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager,"");
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager,"");

	std::string modelPath = MODEL_PATH;
	modelPath += std::string(modelName.begin(), modelName.end());
	bool bSuccess = pImporter->Initialize(modelPath.c_str(), -1, g_pFbxSdkManager->GetIOSettings() );
	if(!bSuccess){
		LOG(L"Failed to load model, Step 1.");
		assert(bSuccess == false);
	}

	bSuccess = pImporter->Import(pFbxScene);
	if(!bSuccess){
		LOG(L"Failed to load model, Step 2");
		assert(bSuccess == false);
	}

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if(pFbxRootNode)
	{
		for(int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if(pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if(AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = (FbxMesh*) pFbxChildNode->GetNodeAttribute();

			int vertexCount = pMesh->GetControlPointsCount();
			FbxVector4* positions = pMesh->GetControlPoints();

			std::vector<RenderVertex> vertexList;
			std::vector<UINT> indexList;

			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				int iNumVertices = pMesh->GetPolygonSize(j);
				assert( iNumVertices == 3 );

				for (int k = 0; k < iNumVertices; k++)
				{
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

					FbxVector4 normal;
					pMesh->GetPolygonVertexNormal(j, vertexList.size(), normal);

					indexList.push_back(pMesh->GetPolygonVertexIndex(j));

					RenderVertex vertex;
					vertex.pos.x = (float)positions[iControlPointIndex].mData[0];
					vertex.pos.y = (float)positions[iControlPointIndex].mData[1];
					vertex.pos.z = (float)positions[iControlPointIndex].mData[2];
					
					vertex.normal.x = normal.mData[0];
					vertex.normal.y = normal.mData[1];
					vertex.normal.z = normal.mData[2];

					vertex.color.x = 1;
					vertex.color.y = 1;
					vertex.color.z = 1;
					vertex.color.w = 1;

					vertex.texCoord.x = 0;
					vertex.texCoord.y = 0;

					vertexList.push_back( vertex );
				}
			}

			RenderVertex* vertices = new RenderVertex[vertexList.size()];
			UINT* indices = new UINT[indexList.size()];

			for(int i = 0; i < indexList.size(); i++){
				indices[i] = i;
			}

			m->vertexBuffer = Mesh::CreateVertexBuffer(vertices, vertexList.size(), VERTEX_TYPE_ALL);
			m->numVertices = vertexList.size();

			m->indexBuffer = Mesh::CreateIndexBuffer(indices, indexList.size());
			m->numIndices = indexList.size();

			m->hasColor = false;
			m->hasNormals = true;
			m->hasPosition = true;
			m->hasTexCoord = false;
			m->meshID = MESH_SHIP;
			m->name = meshNames[m->meshID];

			delete[] vertices;
			delete[] indices;
		}
	}

	return m;
};
