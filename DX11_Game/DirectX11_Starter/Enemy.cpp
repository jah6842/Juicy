#include "Enemy.h"


Enemy::Enemy(MESHES m, MATERIALS mat) : GameObject(m, mat)
{
	dimensions = 4;
	interval = 30;
	
	for (int i = 0; i < dimensions; i++)
	{
		std::vector<Transform> row;

		for (int j = 0; j < dimensions; j++)
		{
			Transform location;
			location.SetPosition(interval * j, 0, interval * i);
			row.push_back(location);
		}

		locations.push_back(row);
	}

	srand(time(0));
	rowIndex = rand() % dimensions;
	columnIndex = rand() % dimensions;

	transform.SetPosition(locations[rowIndex][columnIndex].Pos());
}

void Enemy::Update(float dt)
{
	// Set position
	transform.SetPosition(locations[rowIndex][columnIndex].Pos());
	//checks to see if the enemy is beneith the grid
	/*
	if(transform.PosY <= 0)
	{
		//destroy the enemy
	}
	else
	{

	}*/
}

void Enemy::MoveEnemy(int speed)
{
	
}
