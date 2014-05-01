#include "Ship.h"


Ship::Ship(MESHES m, MATERIALS mat, KeyboardInput* kb) : GameObject(m, mat)
{
	keyboard = kb;
	
	fireMode = FIRE_MODE_NORMAL;

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

	shootCooldown = 0.6f;
	rapidCooldown = 0.3f;
	shootTimer = 0.0f;
}

void Ship::Update(float dt)
{
	shootTimer -= dt;
	
	Move();

	if (keyboard->GetKey(VK_SPACE) && shootTimer <= 0.0f)
	{
		Shoot();
	}

	if (keyboard->GetKeyDown('Z'))
	{
		fireMode = FIRE_MODE_RAPID;
	}
	else if (keyboard->GetKeyDown('X'))
	{
		fireMode = FIRE_MODE_PIERCING;
	}
	else if (keyboard->GetKeyDown('C'))
	{
		fireMode = FIRE_MODE_WIDE;
	}
	else if (keyboard->GetKeyDown('V'))
	{
		fireMode = FIRE_MODE_NORMAL;
	}

	for (UINT i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Update(dt);
		
		if (bullets[i]->CheckOnScreen() == false)
		{
			delete(bullets[i]);
			bullets.erase(bullets.begin() + i);
		}
	}
}

void Ship::Move()
{
	// Check for input
	if (keyboard->GetKeyDown('W'))
	{
		columnIndex++;
	}
	if (keyboard->GetKeyDown('S'))
	{
		columnIndex--;
	}
	if (keyboard->GetKeyDown('A'))
	{
		rowIndex++;
	}
	if (keyboard->GetKeyDown('D'))
	{
		rowIndex--;
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

	transform.SetPosition(locations[rowIndex][columnIndex].Pos());
}

void Ship::Shoot()
{
	Bullet* b;
	
	if (fireMode == FIRE_MODE_PIERCING)
	{
		b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, true);
	}
	else
	{
		b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, false);
	}

	b->transform.SetVelocity(0.0f, 250.0f, 0.0f);
	b->transform.SetScale(1.0f, 1.0f, 1.0f);
	b->transform.SetPosition(transform.Pos());
	bullets.push_back(b);

	if (fireMode == FIRE_MODE_WIDE)
	{
		if (rowIndex > 0)
		{
			b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, false);
			b->transform.SetVelocity(0.0f, 250.0f, 0.0f);
			b->transform.SetScale(1.0f, 1.0f, 1.0f);
			b->transform.SetPosition(locations[rowIndex - 1][columnIndex].Pos());
			bullets.push_back(b);
		}

		if (rowIndex < dimensions - 1)
		{
			b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, false);
			b->transform.SetVelocity(0.0f, 250.0f, 0.0f);
			b->transform.SetScale(1.0f, 1.0f, 1.0f);
			b->transform.SetPosition(locations[rowIndex + 1][columnIndex].Pos());
			bullets.push_back(b);
		}
	}

	if (fireMode == FIRE_MODE_RAPID)
	{
		shootTimer = rapidCooldown;
	}
	else
	{
		shootTimer = shootCooldown;
	}
}
