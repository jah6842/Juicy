#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <string>
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"

enum FireMode
{
	FIRE_MODE_NORMAL,
	FIRE_MODE_RAPID,
	FIRE_MODE_PIERCING,
	FIRE_MODE_WIDE
};

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

#endif // _GAME_OBJECT_H