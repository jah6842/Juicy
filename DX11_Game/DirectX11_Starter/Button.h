#pragma once
#include "GameObject.h"

class Button{

public:
	Transform transform;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

	Button(MESHES m, MATERIALS mat);
	~Button(void);
	boolean Clicked(POINT* p);
	
	float x, y, width, height;
};
