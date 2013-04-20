
#include "Movable.h"

const int Movable::maxHealth = 100;
const int Movable::defaultAttackPower = 25;

Movable::Movable(std::string imgLoc, sf::IntRect collisionBox, sf::IntRect animBox)
	: Entity(imgLoc, collisionBox, animBox, true)
{
	attackPower = defaultAttackPower;
	health = maxHealth;
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