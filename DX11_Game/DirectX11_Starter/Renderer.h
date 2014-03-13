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
	// Draws every registered gameobject
	// Call this once per frame
	static void Draw();

	// Add and remove 
	static void RegisterGameObject(GameObject* go);
	static void UnRegisterGameObject(GameObject* go);

	// Clean up resources
	static void Cleanup();

private:
	static std::unordered_set<GameObject*> registeredGOs;
	static TextureManager* textureManager;
	
	static std::shared_ptr<ConstantBuffer> _perFrameConstantBuffer;
	static std::shared_ptr<ConstantBuffer> _directionalLightBuffer;
	static bool rendererReady;

	Renderer();
	~Renderer();

	// Make sure that we are ready to render
	static void PrepareRenderer();
	static void PrepareLighting();
	static void PrepareMaterial(Material* m);
};

#endif // _RENDERER_H