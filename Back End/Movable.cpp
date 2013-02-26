#include "Movable.h"

#define DEFAULT_HP 100

Movable::Movable()
{
	currentSpeed = 0;
	attackPower = 0;
	currentHP = DEFAULT_HP;
}

// move them to Player class
// void Movable::moveUp()
// {

// }

// void Movable::moveDown()
// {

// }
	
// void Movable::moveLeft()
// {

// }

// void Movable::moveRight()
// {

// }

void Movable::attack()
{

}

double getCurrentSpeed()
{
	return currentSpeed;
}

double getAttackPower()
{
	return attackPower;
}
	
double getCurrentHP()
{
	return currentHP;
}
	
void setCurrentSpeed(double speed)
{
	this->currentSpeed = speed;
}
	
void setAttackPower(double power)
{
	this->attackPower = power;
}
	
void setCurrentHP(double HP)
{
	this->currentHP = HP;
}

