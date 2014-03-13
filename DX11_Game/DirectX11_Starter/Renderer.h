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

	// Add and remove 
	static void RegisterGameObject(GameObject* go);
	static void UnRegisterGameObject(GameObject* go);

private:
	static std::unordered_set<GameObject*> registeredGOs;
	TextureManager* textureManager;
	
	std::shared_ptr<ConstantBuffer> _perFrameConstantBuffer;
	std::shared_ptr<ConstantBuffer> _directionalLightBuffer;
	bool rendererReady;

	//static std::map<MESHES, std::vector<GameObject*>> renderBuckets;

	// Make sure that we are ready to render
	void PrepareRenderer();
	void PrepareLighting();
	void PrepareMaterial(GameObject* go, Material* m);
};

#endif // _RENDERER_H