
#include "Movable.h"
#include "Level.h"

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

void Movable::move(Level &level, sf::Vector2f movement) {
	//Get the sides of the player's collision rectangle
	sf::Vector2f pos = sprite.getPosition();
	float top = pos.y - collisionBox.height/2;
	float left = pos.x - collisionBox.width/2;
	float bot = pos.y + collisionBox.height/2;
	float right = pos.x + collisionBox.width/2;
	sf::Vector2f topLeft(left, top);
	sf::Vector2f botRight(right, bot);
	sf::Vector2f topRight(right, top);
	sf::Vector2f botLeft(left, bot);
	//printf("(%g, %g), (%g, %g)\n", topLeft.x, topLeft.y, botRight.x, botRight.y);
	int nearestTopLeft, nearestBotRight, nearest;
	bool foundTopLeft, foundBotRight, found;

	if(movement.y<0) {
		foundTopLeft = level.GetCollide(topLeft, false, false, nearestTopLeft);
		foundBotRight = level.GetCollide(topRight, false, false, nearestBotRight);
		nearest = std::max(nearestTopLeft, nearestBotRight);
		nearest -= (int)top;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = -INT_MAX;
		//printf("Nearest Up: %d\n", nearest);
		movement.y = std::max(movement.y, (float)nearest);
	}
	if(movement.y>0) {
		foundTopLeft = level.GetCollide(botLeft, false, true, nearestTopLeft);
		foundBotRight = level.GetCollide(botRight, false, true, nearestBotRight);
		nearest = std::min(nearestTopLeft, nearestBotRight);
		nearest -= (int)bot;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = INT_MAX;
		//printf("Nearest Down: %d\n", nearest);
		movement.y = std::min(movement.y, (float)nearest);
	}
	if(movement.x<0) {
		foundTopLeft = level.GetCollide(topLeft, true, false, nearestTopLeft);
		foundBotRight = level.GetCollide(botLeft, true, false, nearestBotRight);
		nearest = std::max(nearestTopLeft, nearestBotRight);
		nearest -= (int)left;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = -INT_MAX;
		//printf("Nearest Left: %d\n", nearest);
		movement.x = std::max(movement.x, (float)nearest);
	}
	if(movement.x>0) {
		foundTopLeft = level.GetCollide(topRight, true, true, nearestTopLeft);
		foundBotRight = level.GetCollide(botRight, true, true, nearestBotRight);
		nearest = std::min(nearestTopLeft, nearestBotRight);
		nearest -= (int)right;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = INT_MAX;
		//printf("Nearest Right: %d\n", nearest);
		movement.x = std::min(movement.x, (float)nearest);
	}

	sprite.move(movement);
}
