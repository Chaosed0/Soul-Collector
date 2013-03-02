
#include "Demon.h"
#include "Level.h"

Demon::Demon(sf::Vector2f pos)
	: Movable("assets/img/zombie_topdown.png", sf::IntRect(20, 20, 27, 23), sf::IntRect(0, 0, 64, 64))
{
	sprite.setPosition(pos);
	moveHoriz = rand()%2;
	movePos = rand()%2;

	ModifyAnimSet("idle", 0, 3, true);
	AddAnimSet("walk", 4, 11, true);
}

void Demon::Update(const Level& level)
{
	PlayAnim("walk");

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
	sf::Vector2f movement(0, 0);

	if(!moveHoriz && movePos) {
		foundTopLeft = level.GetCollide(topLeft, false, false, nearestTopLeft);
		foundBotRight = level.GetCollide(topRight, false, false, nearestBotRight);
		nearest = std::max(nearestTopLeft, nearestBotRight);
		nearest -= (int)top;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = -INT_MAX;
		//printf("Nearest Up: %d\n", nearest);
		movement.y = std::max(-moveSpeed, (float)nearest);
	}
	if(!moveHoriz && !movePos) {
		foundTopLeft = level.GetCollide(botLeft, false, true, nearestTopLeft);
		foundBotRight = level.GetCollide(botRight, false, true, nearestBotRight);
		nearest = std::min(nearestTopLeft, nearestBotRight);
		nearest -= (int)bot;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = INT_MAX;
		//printf("Nearest Down: %d\n", nearest);
		movement.y = std::min(moveSpeed, (float)nearest);
	}
	if(moveHoriz && !movePos) {
		foundTopLeft = level.GetCollide(topLeft, true, false, nearestTopLeft);
		foundBotRight = level.GetCollide(botLeft, true, false, nearestBotRight);
		nearest = std::max(nearestTopLeft, nearestBotRight);
		nearest -= (int)left;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = -INT_MAX;
		//printf("Nearest Left: %d\n", nearest);
		movement.x += std::max(-moveSpeed, (float)nearest);
	}
	if(moveHoriz && movePos) {
		foundTopLeft = level.GetCollide(topRight, true, true, nearestTopLeft);
		foundBotRight = level.GetCollide(botRight, true, true, nearestBotRight);
		nearest = std::min(nearestTopLeft, nearestBotRight);
		nearest -= (int)right;
		found = foundTopLeft || foundBotRight;
		if(!found)
			nearest = INT_MAX;
		//printf("Nearest Right: %d\n", nearest);
		movement.x += std::min(moveSpeed, (float)nearest);
	}

	/*if(!movement.x && !movement.y) {
		if(moveHoriz && movePos) {
			moveHoriz = false;
		} else if (!moveHoriz && movePos) {
			moveHoriz = true;
			movePos = false;
		} else if(moveHoriz && !movePos) {
			moveHoriz = false;
		} else {
			moveHoriz = true;
			movePos = true;
		}
	}*/

	if(rand()%100 > 95) {
		moveHoriz = rand()%2;
		movePos = rand()%2;
	}

	sprite.move(movement);

	if(moveHoriz) {
		if(movePos) {
			sprite.setRotation(90);
		} else {
			sprite.setRotation(270);
		}
	} else {
		if(movePos) {
			sprite.setRotation(0);
		} else {
			sprite.setRotation(180);
		}
	}
}