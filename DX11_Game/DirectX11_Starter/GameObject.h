#pragma once

#include <string>
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"

class GameObject {

public:
	Transform transform;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

	GameObject();
	GameObject(MESHES m, MATERIALS mat);
	~GameObject();

	virtual void Update(float dt);
};

