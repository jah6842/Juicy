#include "Ship.h"


Ship::Ship(MESHES m, MATERIALS mat, KeyboardInput* kb) : GameObject(m, mat)
{
	keyboard = kb;
	
	fireMode = FIRE_MODE_NORMAL;

	dimensions = 4;
	interval = 30;

	lives = 3;
	rapidEnergy = 0.0f;
	wideEnergy = 0.0f;
	piercingEnergy = 0.0f;
	
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

	shield = false;
	powerup = false;
	specialLength = 10.0f;
}

void Ship::Update(float dt)
{
	shootTimer -= dt;
	
	Move();

	if (keyboard->GetKey(VK_SPACE) && shootTimer <= 0.0f && shield == false)
	{
		Shoot();
	}

	if (keyboard->GetKeyDown('Z') && rapidEnergy == 100 && powerup == false)
	{
		fireMode = FIRE_MODE_RAPID;
		powerup = true;
	}
	else if (keyboard->GetKeyDown('X') && piercingEnergy == 100 && powerup == false)
	{
		fireMode = FIRE_MODE_PIERCING;
		powerup = true;
	}
	else if (keyboard->GetKeyDown('C') && wideEnergy == 100 && powerup == false)
	{
		fireMode = FIRE_MODE_WIDE;
		powerup = true;
	}

	if (powerup)
	{
		if (fireMode == FIRE_MODE_RAPID)
		{
			rapidEnergy -= dt * specialLength;

			if (rapidEnergy <= 0)
			{
				rapidEnergy = 0;
				powerup = false;
				fireMode = FIRE_MODE_NORMAL;
			}
		}
		else if (fireMode == FIRE_MODE_PIERCING)
		{
			piercingEnergy -= dt * specialLength;

			if (piercingEnergy <= 0)
			{
				piercingEnergy = 0;
				powerup = false;
				fireMode = FIRE_MODE_NORMAL;
			}
		}
		else if (fireMode == FIRE_MODE_WIDE)
		{
			wideEnergy -= dt * specialLength;

			if (wideEnergy <= 0)
			{
				wideEnergy = 0;
				powerup = false;
				fireMode = FIRE_MODE_NORMAL;
			}
		}
	}

	for (UINT i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Update(dt);
		
		if (bullets[i]->CheckOnScreen() == false || bullets[i]->CheckCollision() == true)
		{
			delete(bullets[i]);
			bullets.erase(bullets.begin() + i);
		}
	}

	shield = keyboard->GetKey(VK_SHIFT);
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
		b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex, columnIndex, FIRE_MODE_PIERCING);
	}
	else
	{
		b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex, columnIndex, FIRE_MODE_NORMAL);
	}

	b->transform.SetVelocity(0.0f, 250.0f, 0.0f);
	b->transform.SetScale(1.0f, 1.0f, 1.0f);
	b->transform.SetPosition(transform.Pos());
	bullets.push_back(b);

	if (fireMode == FIRE_MODE_WIDE)
	{
		if (rowIndex > 0)
		{
			b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex - 1, columnIndex, FIRE_MODE_WIDE);
			b->transform.SetVelocity(0.0f, 250.0f, 0.0f);
			b->transform.SetScale(1.0f, 1.0f, 1.0f);
			b->transform.SetPosition(locations[rowIndex - 1][columnIndex].Pos());
			bullets.push_back(b);
		}

		if (rowIndex < dimensions - 1)
		{
			b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex + 1, columnIndex, FIRE_MODE_WIDE);
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

vector<Bullet*> Ship::GetBullets()
{
	return bullets;
}

int Ship::GetRow()
{
	return rowIndex;
}

int Ship::GetColumn()
{
	return columnIndex;
}

void Ship::Draw(Renderer* renderer)
{
	std::string lifeString = "Lives: " + std::to_string(lives);
	std::string rapidString = "Rapid Fire: " + std::to_string(rapidEnergy);
	std::string piercingString = "Piercing Fire: " + std::to_string(piercingEnergy);
	std::string wideString = "Wide Fire: " + std::to_string(wideEnergy);

	/*for (int i = 0; i < lives; i++)
	{
		strcat(lifeString, "I");
	}*/
	
	renderer->DrawString(lifeString.c_str(), 10.0f, 10.0f, 48.0f);

	if (rapidEnergy >= 100)
	{
		renderer->DrawString(rapidString.c_str(), 10.0f, 1000.0f, 48.0f, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		renderer->DrawString(rapidString.c_str(), 10.0f, 1000.0f, 48.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (piercingEnergy >= 100)
	{
		renderer->DrawString(piercingString.c_str(), 10.0f, 1050.0f, 48.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		renderer->DrawString(piercingString.c_str(), 10.0f, 1050.0f, 48.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (wideEnergy >= 100)
	{
		renderer->DrawString(wideString.c_str(), 10.0f, 1100.0f, 48.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	}
	else
	{
		renderer->DrawString(wideString.c_str(), 10.0f, 1100.0f, 48.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (shield)
	{
		UINT screenWidth, screenHeight;
		Camera::MainCamera.GetScreenSize(screenWidth,screenHeight);

		float x = (700.0f / 800.0f) * (float)screenWidth;
		float y = (900.0f / 600.0f) * (float)screenHeight;

		renderer->DrawString("SHIELD", x, y, 60.0f);
	}
}

void Ship::Collision(Bullet* bullet)
{
	if (shield)
	{
		if (bullet->GetUpgradeType() == FIRE_MODE_NORMAL)
		{
			piercingEnergy += 10.0f;

			if (piercingEnergy > 100)
			{
				piercingEnergy = 100.0f;
			}
		}
		else if (bullet->GetUpgradeType() == FIRE_MODE_RAPID)
		{
			rapidEnergy += 10.0f;

			if (rapidEnergy > 100)
			{
				rapidEnergy = 100.0f;
			}
		}
		else if (bullet->GetUpgradeType() == FIRE_MODE_WIDE)
		{
			wideEnergy += 10.0f;

			if (wideEnergy > 100)
			{
				wideEnergy = 100.0f;
			}
		}
	}
	else
	{
		lives--;
		rowIndex = rand() % dimensions;
		columnIndex = rand() % dimensions;
	}
}

bool Ship::IsDead()
{
	if (lives < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
