
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
	//This activatable is different from others; when activated, it stays activated.
	//We are going to hack this a bit; no other activatable can be both activated
	// and expended at the same time, so when the level sees an activatable that is
	// both, it will know that the activatable is activated stairs.
	isFinished = true;
}

std::string Stairs::GetNextLevel()
{
	return nextLevel;
}

std::string Stairs::GetNextSpawn()
{
	return nextSpawn;
}