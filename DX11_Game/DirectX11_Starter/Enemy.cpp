#include "Enemy.h"


Enemy::Enemy(MESHES m, MATERIALS mat) : GameObject(m, mat)
{
	dimensions = 4;
	interval = 30;
	active = true;
	
	for (int i = 0; i < dimensions; i++)
	{
		std::vector<Transform> row;

		for (int j = 0; j < dimensions; j++)
		{
			Transform location;
			location.SetPosition(interval * j, 300, interval * i);
			row.push_back(location);
		}

		locations.push_back(row);
	}

	srand(time(0));
	rowIndex = rand() % dimensions;
	columnIndex = rand() % dimensions;

	transform.SetPosition(locations[rowIndex][columnIndex].Pos());
	transform.SetScale(3,3,3);
	transform.SetRotation(XMConvertToRadians(90),0,0);
	transform.AddVelocity(0.0,-30.0,0.0);
}

Enemy::~Enemy() 
{
	// Unregister this GameObject from the renderer
	//Renderer::UnRegisterGameObject(this);
}

bool Enemy::getActive()
{
	return active;
}
void Enemy::Update(float dt)
{
	
	//checks to see if the enemy is beneith the grid
	
	if(transform.PosY() <= 0)
	{
		active = false;
		
	}
	transform.Update(dt);
}
