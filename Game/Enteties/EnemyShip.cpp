#include "EnemyShip.h"
#include "cstdlib"
#include "Time.h"
#include "Laser.h"

int EnemyShip::EnemyShipCount = 0;

EnemyShip::EnemyShip(Vector2* pos) : Ship()
{
	//new RigidBody(this, 0.9, 0.3, false, new BoxCollider(sprite));

	maxhp = 3;
	hp = maxhp;
	
	EnemyShipCount++;
	laserSprite = Sprite::GetSprite("EnemyLaser");
	RandTimeForShot();

	spawnedPos = *transform->position;
    transform->position->y = pos->y + 2;
    transform->position->x = pos->x + 0.8;

	distToText = *new Vector2(0, 0.4);
	hpText = new Text(std::to_string(hp) + "/" + std::to_string(maxhp), { 255,255,255,255 }, "Cut_Deep", 1, *pos + distToText);
}

EnemyShip::~EnemyShip()
{
	delete hpText;
}

double sideSpeed = 2;
void EnemyShip::Update()
{
	if (timeSinceShot < randTime)
	{
		timeSinceShot += Time::fixedDeltaTime;
	}
	else if (GameManager::instance->getGameState() == false)
	{
		timeSinceShot = 0;
		RandTimeForShot();

        //TODO: fix here
		//new laser("EnemyLaser", laserSprite, -7, new Vector2(*holderObject->transform->position), new Vector2(1, 1));
	}
	
	double dist = Vector2::Distance(spawnedPos, *transform->position);
	if (dist > 0.05)
	{
		Vector2 dir = Vector2::Normalize(*gameObject->transform->position - spawnedPos) * -dist * Time::fixedDeltaTime * 1.5;
		//rigidBody->velocity.y = dir.y;
	}
	else
	{
		//rigidBody->velocity.y = 0;
	}

	if (transform->position->x - spawnedPos.x > 0.7)
	{
		sideSpeed = -2;
	}
	else if (transform->position->x - spawnedPos.x < -0.7)
	{
		sideSpeed = 2;
	}

	//rigidBody->velocity.x = sideSpeed * Time::fixedDeltaTime;

	hpText->position = *transform->position + distToText;
}

void EnemyShip::Damage(int dmg)
{
	hp -= dmg;

	hpText->text = std::to_string(hp) + "/" + std::to_string(maxhp);

	if (hp <= 0)
	{
		EnemyShipCount--;

		GameManager::instance->removeEnemyShip(this);
		GameManager::instance->CheckGameState();
		
		delete this;
	}
}

void EnemyShip::RandTimeForShot()
{
	randTime = (rand() * 1.0 / RAND_MAX) * 1.5 + 1;
}
