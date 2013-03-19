
#include "Demon.h"
#include "Level.h"

const sf::Time Demon::alertTime = sf::milliseconds(1500);
const sf::Time Demon::chargeTime = sf::milliseconds(5000);

Demon::Demon(sf::Vector2f pos)
	: Movable("assets/img/zombie_topdown.png", sf::IntRect(20, 20, 27, 23), sf::IntRect(0, 0, 64, 64))
{
	moveSpeed = 1.0f;
	sprite.setPosition(pos);
	movement = sf::Vector2f(0, 0);
	state = IDLE;

	timer.restart();

	ModifyAnimSet("idle", 0, 3, true);
	AddAnimSet("walk", 4, 11, true);
}

void Demon::Update(const Level& level)
{
	//Check if the demon's been spotted by the player
	sf::Vector2f playerPos = level.GetPlayer().GetPos();
	sf::Vector2f relDist = playerPos - GetPos();
	float dist = magnitude(relDist);

	sf::Int64 time = timer.getElapsedTime().asMicroseconds();

	//Demon behavior controls
	
	if(state == CHARGING && time > chargeTime.asMicroseconds()) {
		//If the demon's charged for long enough, stop charging
		moveSpeed = 1.0f;
		timer.restart();
		state = IDLE;
	}

	if((state != ALERT && state != CHARGING) && dist < SPOT_RADIUS) {
		//If the player is close to the demon, put on alert
		state = ALERT;
		timer.restart();
	} else if(state == ALERT && dist > SPOT_RADIUS) {
		//If the player moved out of the monster's visibility range, then go back to
		// idling
		moveSpeed = 1.0f;
		state = IDLE;
		timer.restart();
	} else if((state != CHARGING && dist < CHARGE_RADIUS) || (state == ALERT && time > alertTime.asMicroseconds())) {
		//If the player was close to the demon for too long, or got inside their
		// charge radius, start charging at where the player currently is
		moveSpeed = 5.0f;
		state = CHARGING;
		movement = relDist*(moveSpeed/dist);
		timer.restart();
	}

	if(state == MOVING || state == CHARGING) {
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
		sprite.setRotation(360/(2*PI)*atan2f(movement.y, movement.x)+90);
	} else if(state == IDLE) {
		PlayAnim("idle");
	} else if(state == ALERT) {
		//Change to alert animation later
		PlayAnim("idle");
		//Rotate the sprite to face the player
		sprite.setRotation(360/(2*PI)*atan2f(relDist.y, relDist.x)+90);
	}

	//If the monster was charging and collided into a wall, stop charging
	if(state == CHARGING && (movement.x == 0 || movement.y == 0)) {
		state = IDLE;
		timer.restart();
	}

	if((state == MOVING || state == IDLE) && (float)rand()/(float)RAND_MAX > 0.95f) {
		float angle = ((float)rand()/(float)RAND_MAX)*2*PI;
		movement.x = moveSpeed*cos(angle);
		movement.y = moveSpeed*sin(angle);
		//ARGH CONSTANTS
		bool isMoving = ((float)rand()/(float)RAND_MAX > 0.90f);
		if(isMoving) {
			state = MOVING;
		} else {
			state = IDLE;
		}
		//printf("New movement: (%g, %g)\n", movement.x, movement.y);
	}
}