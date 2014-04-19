#pragma once

#include <d3d11.h>
#include <DirectXColors.h>
#include <map>
#include <unordered_set>
#include <vector>
#include <list>
#include <algorithm>


#include "GameObject.h"
#include "TextureManager.h"
#include "Loader.h"



class Button;
class GameObject;

class Renderer {
public:

	Renderer();
	~Renderer();

	// Draws every registered gameobject
	// Call this once per frame
	void Draw();

	// Draw text
	void DrawString(const char* text, float x, float y, float size = 24, XMFLOAT4 color = XMFLOAT4(1,1,1,1));

	void DrawButton(GameObject* b);

	// Add and remove 
	static void RegisterGameObject(GameObject* go);
	static void UnRegisterGameObject(GameObject* go);

	static void Register2DGameObject(GameObject* go);
	static void UnRegister2DGameObject(GameObject* go);

private:
	static std::unordered_set<GameObject*> registeredGOs;
	static std::unordered_set<GameObject*> registered2DGOs;
	TextureManager* textureManager;
	
	std::shared_ptr<ConstantBuffer> _perFrameConstantBuffer;
	std::shared_ptr<ConstantBuffer> _directionalLightBuffer;
	bool rendererReady;

	// Make sure that we are ready to render
	void PrepareRenderer();
	void PrepareLighting();
	void PrepareMaterial(GameObject* go, Material* m);

	// Text drawing variables
	std::shared_ptr<Material>	_textMaterial;
	ID3D11Buffer*				fontVBuffer;

	//2D Stuff
	void draw2D(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
};
