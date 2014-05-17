#include "Enemy.h"


Enemy::Enemy(MESHES m, MATERIALS mat, bool shooter) : GameObject(m, mat)
{
	dimensions = 4;
	interval = 30;
	active = true;
	canShoot = shooter;
	shotCooldown = 180;
	rapidCooldown = 40;
	burst = 0;
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

	//setup marker
	
	marker = new GameObject(MESH_CUBE, MATERIAL_MARKER);
	marker ->transform.SetPosition(columnIndex * interval - interval/4, -9.0f, rowIndex * interval-interval/4);
	//printf("%d, %d, %d\n", locations[rowIndex][columnIndex].Pos().x, locations[rowIndex][columnIndex].Pos().y, locations[rowIndex][columnIndex].Pos().z);
	//marker->transform.SetPosition(0,-4.8f,0);
	marker ->transform.SetScale(10,10,10);
	//marker->transform.SetRotationalVelocity(0,0,0);
	//Renderer::RegisterGameObject(marker);
}

Enemy::~Enemy() 
{
	// Unregister this GameObject from the renderer
	//Renderer::UnRegisterGameObject(this);
	//Renderer::UnRegisterGameObject(marker);
	delete marker;
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
	marker->transform.Update(dt);
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
		b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex, columnIndex, FIRE_MODE_PIERCING);
	}
	else if (fireMode == FIRE_MODE_WIDE)
	{
		b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex, columnIndex, FIRE_MODE_WIDE);
	}
	else if (fireMode == FIRE_MODE_RAPID)
	{
		b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex, columnIndex, FIRE_MODE_RAPID);
	}
	else 
	{
		b = new Bullet(MESH_CUBE, MATERIAL_DEFAULT, rowIndex, columnIndex, FIRE_MODE_NORMAL);
	}

	b->transform.SetVelocity(0.0f, -250.0f, 0.0f);
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
		shotCooldown = rapidCooldown;
		burst++;
		if(burst > 3)
		{
			shotCooldown = regCooldown;
			burst = 0;
		}
	}
	else
	{
		shotCooldown = regCooldown;
	}
}

std::vector<Bullet*> Enemy::GetBullets()
{
	return bullets;
}