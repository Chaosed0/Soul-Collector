
#include "Stairs.h"
#include "Level.h"

Stairs::Stairs(const sf::Vector2f& pos, const std::string& nextLevel, const std::string& nextSpawn)
	: Activatable("assets/img/stairs.png", sf::IntRect(0, 0, 32, 32), sf::IntRect(0, 0, 32, 32)) 
{
	sprite.setPosition(pos);
	this->nextLevel = nextLevel;
	this->nextSpawn = nextSpawn;
}

void Stairs::Update(Level& level, const sf::Time& timePassed)
{
	if(IsActive()) {
		//The beginning of the end...
		level.SetLevelTransition(nextLevel, nextSpawn);
		isActive = false;
	}
}

std::string Stairs::GetNextLevel()
{
	return nextLevel;
}

std::string Stairs::GetNextSpawn()
{
	return nextSpawn;
}