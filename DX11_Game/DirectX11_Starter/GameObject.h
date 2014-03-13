#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

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
	//Material* material;

	GameObject();
	GameObject(MESHES m, MATERIAL_DESCRIPTION mat);
	~GameObject();

	void Update(float dt);
};

#endif // _GAME_OBJECT_H