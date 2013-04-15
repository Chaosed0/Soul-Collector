
#include "Movable.h"

Movable::Movable(std::string imgLoc, sf::IntRect collisionBox, sf::IntRect animBox)
	: Entity(imgLoc, collisionBox, animBox, true)
{
	attackPower = 25;
	health = 100;
}

void Movable::Attack(Movable& movable)
{
	movable.RemoveHealth(attackPower);
	//printf("Hit Movable\n");
}

void Movable::RemoveHealth(int amount)
{
	health -= amount;
}

bool Movable::IsAlive()
{
	return health > 0;
}