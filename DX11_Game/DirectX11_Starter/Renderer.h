#ifndef _RENDERER_H
#define _RENDERER_H

#include <d3d11.h>
#include <map>
#include <unordered_set>
#include <vector>
#include <list>
#include <algorithm>

#include "GameObject.h"

class GameObject;

class Renderer {
private:
	static std::unordered_set<GameObject*> registeredGOs;

	static ID3D11Buffer* _perFrameConstantBuffer;
	static bool rendererReady;

	Renderer();
	~Renderer();

	// Internal drawing functions
	static void DrawSingle(GameObject* go);
	static void DrawBatched();

	// Make sure that we are ready to render
	static void PrepareRenderer();
	

public:
	// Draws every registered gameobject
	// Call this once per frame
	static void Draw();

	// Add and remove 
	static void RegisterGameObject(GameObject* go);
	static void UnRegisterGameObject(GameObject* go);

	// Clean up resources
	static void Cleanup();
};

#endif // _RENDERER_H