#pragma once
#include "GameObject.h"

class Button : public GameObject{

public:


	Button(MESHES m, MATERIALS mat);
	~Button(void);
	boolean Clicked(POINT* p);
	
	float x, y, width, height;
};
