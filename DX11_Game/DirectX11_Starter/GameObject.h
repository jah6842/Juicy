#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <string>
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
// Currently used to randomly move position, will have a library for this later.
#include <random>

class GameObject {

public:
	Transform transform;
	Mesh* mesh;
	Material* material;

	GameObject();
	GameObject(std::wstring meshName);
	~GameObject();

	void Update(float dt);
};

#endif // _GAME_OBJECT_H