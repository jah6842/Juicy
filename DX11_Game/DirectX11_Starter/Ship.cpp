#include "Ship.h"


Ship::Ship(MESHES m, MATERIALS mat) : GameObject(m, mat)
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
	//transform.SetPosition(60.0f, 60.0f, 60.0f);
}

void Ship::Update(float dt)
{
	// Check for input
	if (GetAsyncKeyState('W'))
	{
		columnIndex++;
	}
	if (GetAsyncKeyState('S'))
	{
		columnIndex--;
	}
	if (GetAsyncKeyState('A'))
	{
		rowIndex--;
	}
	if (GetAsyncKeyState('D'))
	{
		rowIndex++;
	}

	// Stay in bounds and wrap
	if (columnIndex >= dimensions)
	{
		columnIndex = 0;
	}
	else if (columnIndex < 0)
	{
		columnIndex = dimensions - 1;
	}

	if (rowIndex >= dimensions)
	{
		rowIndex = 0;
	}
	else if (rowIndex < 0)
	{
		rowIndex = dimensions - 1;
	}

	// Set position
	transform.SetPosition(locations[rowIndex][columnIndex].Pos());
}
