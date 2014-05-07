#include "Enemy.h"


Enemy::Enemy(MESHES m, MATERIALS mat, bool shooter) : GameObject(m, mat)
{
	dimensions = 4;
	interval = 30;
	active = true;
	canShoot = shooter;
	shotCooldown = 180;
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
	
	shotCooldown = 0.0f;
	regCooldown = 3.6f;
	rapidCooldown = 1.3f;

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

void Enemy::setActive(bool isActive)
{
	active = isActive;
}

void Enemy::Update(float dt)
{
	//shooting stuff
	if(canShoot){
		shotCooldown -= dt;
		if(shotCooldown <= 0)
		{
			shoot();
			
		}
	}

	//move bullets
	
	for (UINT i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Update(dt);
		
		if (bullets[i]->CheckOnScreen() == false || bullets[i]->CheckCollision() == true)
		{
			delete(bullets[i]);
			bullets.erase(bullets.begin() + i);
		}
	}
	//checks to see if the enemy is beneith the grid
	
	if(transform.PosY() <= 0)
	{
		active = false;
	}
	transform.Update(dt);
}

int Enemy::GetRow()
{
	return rowIndex;
}

int Enemy::GetColumn()
{
	return columnIndex;
}
void Enemy::shoot()
{
	Bullet* b;
	
	if (fireMode == FIRE_MODE_PIERCING)
	{
		b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex, columnIndex, true);
	}
	else
	{
		b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex, columnIndex, false);
	}

	b->transform.SetVelocity(0.0f, -250.0f, 0.0f);
	b->transform.SetScale(1.0f, 1.0f, 1.0f);
	b->transform.SetPosition(transform.Pos());
	bullets.push_back(b);

	if (fireMode == FIRE_MODE_WIDE)
	{
		if (rowIndex > 0)
		{
			b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex - 1, columnIndex, false);
			b->transform.SetVelocity(0.0f, 250.0f, 0.0f);
			b->transform.SetScale(1.0f, 1.0f, 1.0f);
			b->transform.SetPosition(locations[rowIndex - 1][columnIndex].Pos());
			bullets.push_back(b);
		}

		if (rowIndex < dimensions - 1)
		{
			b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex + 1, columnIndex, false);
			b->transform.SetVelocity(0.0f, 250.0f, 0.0f);
			b->transform.SetScale(1.0f, 1.0f, 1.0f);
			b->transform.SetPosition(locations[rowIndex + 1][columnIndex].Pos());
			bullets.push_back(b);
		}
	}

	if (fireMode == FIRE_MODE_RAPID)
	{
		shotCooldown = rapidCooldown;
	}
	else
	{
		shotCooldown = regCooldown;
	}
}