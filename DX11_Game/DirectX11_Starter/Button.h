#pragma once
#include "GameObject.h"

class Button : public GameObject{

public:


	Button(MESHES m, MATERIALS mat, XMFLOAT4 dimensions);
	~Button(void);
	boolean Clicked(POINT* p);
	boolean Visible;
private:
	float x, y, width, height;
};