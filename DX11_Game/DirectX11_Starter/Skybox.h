#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "GameObject.h"

class Skybox : public GameObject {

public:
	Skybox(MATERIALS mat);
	~Skybox();

	void Update(float dt);
};

#endif // _SKYBOX_H