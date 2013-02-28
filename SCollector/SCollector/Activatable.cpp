
#include "Activatable.h"

Activatable::Activatable(std::string imgLoc, sf::IntRect collisionBox, sf::IntRect animBox)
	: Entity(imgLoc, collisionBox, animBox)
{
	isActive = false;
	isFinished = false;
}

void Activatable::Activate()
{
	isActive = true;
}

bool Activatable::IsActive()
{
	return isActive;
}

void Activatable::Finish()
{
	isFinished = true;
	isActive = false;
}

bool Activatable::IsFinished()
{
	return isFinished;
}