#ifndef _RENDERER_H
#define _RENDERER_H

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

	//Draw Button
	void DrawButton(GameObject* b);

	//Draw Particles
	void DrawParticles(GameObject* p);

	// Add and remove 
	static void RegisterGameObject(GameObject* go);
	static void UnRegisterGameObject(GameObject* go);

private:
	static std::unordered_set<GameObject*> registeredGOs;
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
};

#endif // _RENDERER_H