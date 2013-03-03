
#include "Demon.h"
#include "Level.h"

Demon::Demon(sf::Vector2f pos)
	: Movable("assets/img/zombie_topdown.png", sf::IntRect(20, 20, 27, 23), sf::IntRect(0, 0, 64, 64))
{
	moveSpeed = 1.0f;
	sprite.setPosition(pos);
	movement = sf::Vector2f(0, 0);
	moving = true;
	spotted = false;

	ModifyAnimSet("idle", 0, 3, true);
	AddAnimSet("walk", 4, 11, true);
}

void Demon::Update(const Level& level)
{
	//Check if the demon's been spotted by the player
	sf::Vector2f playerPos = level.GetPlayer().GetPos();
	sf::Vector2f relDist = playerPos - GetPos();
	float dist = sqrt(relDist.x*relDist.x + relDist.y*relDist.y);
	if(dist < SPOT_RADIUS) {
		moveSpeed = 1.5f;
		spotted = true;
		moving = true;
	} else {
		moveSpeed = 1.0f;
		spotted = false;
	}

	if(moving) {
		PlayAnim("walk");

		//If the demon's been spotted, make it follow the player instead of
		// wandering around
		if(spotted) {
			movement = relDist*(moveSpeed/dist);
		}

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
		sprite.setRotation(360/(2*3.14159)*atan2f(movement.y, movement.x)+90);
	} else {
		PlayAnim("idle");
	}

	if(!spotted && rand()%100 > 95) {
		float angle = ((float)rand()/(float)RAND_MAX)*2*3.14159;
		movement.x = moveSpeed*cos(angle);
		movement.y = moveSpeed*sin(angle);
		moving = rand()%2;
		//printf("New movement: (%g, %g)\n", movement.x, movement.y);
	}
}