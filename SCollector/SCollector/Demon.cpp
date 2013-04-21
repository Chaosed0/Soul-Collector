
#include "Demon.h"
#include "Level.h"
#include "Player.h"

const sf::Time Demon::alertTime = sf::milliseconds(1000);
const sf::Time Demon::chargeTime = sf::milliseconds(5000);
const sf::Time Demon::stunTime = sf::milliseconds(500);
const float Demon::wanderSpeed = 60.0f;
const float Demon::chargeSpeed = 360.0f;

Demon::Demon(sf::Vector2f pos)
	: Movable("assets/img/zombie_topdown.png", sf::IntRect(20, 20, 27, 23), sf::IntRect(0, 0, 64, 64))
{
	sprite.setPosition(pos);
	state = IDLE;

	timer = sf::Time::Zero;

	animManager.ModifyAnimSet("idle", 0, 0, true);
	animManager.AddAnimSet("alert", 0, 3, true);
	animManager.AddAnimSet("walk", 4, 11, true);
	animManager.AddAnimSet("death", 21, 27, false);

	soundManager.AddSound("assets/sound/charge.ogg", "charge", true);
	soundManager.AddSound("assets/sound/alert.ogg", "alert", false);
	soundManager.AddSound("assets/sound/thwack.ogg", "hit", false);
}

void Demon::Attack(Movable& movable)
{
	Movable::Attack(movable);
	state = RECOVERING;
	timer = sf::Time::Zero;
	soundManager.StopSound("charge");
	soundManager.PlaySound("hit");
}

void Demon::Update(Level& level, const sf::Time& timePassed)
{
	//First, before anything, check if the demon is dead
	if(!IsAlive()) {
		//Since the PlayAnim() function simply stops on the last frame for non-looping
		// animations, we don't have to do anything to stop the animation playing
		PlayAnim("death", timePassed);
		state = DEAD;
		soundManager.StopLastSound();
		//Rather than encompassing everything else in here with a huge if statement,
		// just return from the function; much cleaner
		return;
	}

	//Check if the player's been spotted by the demon
	//First, make sure the player can be seen
	sf::Vector2f playerPos = level.GetPlayer().GetPos();
	sf::Vector2f relDist = playerPos - GetPos();
	float dist = magnitude(relDist);
	sf::Vector2f nearest;
	level.GetCollide(GetPos(), atan2(relDist.y, relDist.x), nearest);
	float distFromWall = magnitude(GetPos()-nearest);
	bool canSeePlayer = dist <= distFromWall;
	//printf("player:(%g,%g), nearest:(%g,%g), angle: %g\n", playerPos.x, playerPos.y, nearest.x, nearest.y, atan2(relDist.y, relDist.x)*TO_DEG);
	//printf("dist:%g, nearest:%g, cansee:%d\n", dist, distFromWall, canSeePlayer);
	float moveSpeed = 0.0f;
	sf::Vector2f movement;

	/*shape.setPointCount(4);
	shape.setPoint(0, GetPos()-sf::Vector2f(1,1));
	shape.setPoint(1, GetPos()+sf::Vector2f(1,1));
	shape.setPoint(2, nearest+sf::Vector2f(1,1));
	shape.setPoint(3, nearest-sf::Vector2f(1,1));
	PlayAnim("idle", timePassed);
	return;*/


	timer += timePassed;

	//Demon behavior controls
	
	if(state == CHARGING && timer > chargeTime) {
		//If the demon's charged for long enough, stop charging
		state = RECOVERING;
		soundManager.StopSound("charge");
		//printf("Demon charge timer expired\n");
		timer = sf::Time::Zero;
	} else if(state == RECOVERING && timer > stunTime) {
		//If the monster hit a wall and is finished recovering, go back to idle
		state = IDLE;
		timer = sf::Time::Zero;
	}

	if((state == IDLE || state == MOVING) && canSeePlayer && dist < SPOT_RADIUS) {
		//If the player is close to the demon, put on alert
		state = ALERT;
		soundManager.PlaySound("alert");
		//printf("Demon is alerted\n");
		timer = sf::Time::Zero;
	} else if(state == ALERT && timer > alertTime) {
		//If the player was close to the demon for too long, or got inside their
		// charge radius, start charging at where the player currently is
		state = CHARGING;
		soundManager.PlaySound("charge");
		moveAngle = atan2f(relDist.y, relDist.x);
		//printf("Demon is charging player after %g seconds\n", timer.asSeconds());
		timer = sf::Time::Zero;
	} else if(state == ALERT && (dist >= SPOT_RADIUS || !canSeePlayer)) {
		//If the player moved out of the monster's visibility range, then go back to
		// idling
		state = IDLE;
		//printf("Demon is no longer alerted\n");
		timer = sf::Time::Zero;
	} else if(state == ALERT) {
		//If the demon is simply alert, turn it to face the player
		moveAngle = atan2f(relDist.y, relDist.x);
	}
	
	if(state == MOVING || state == IDLE) {
		moveSpeed = wanderSpeed*timePassed.asSeconds();
	} else if(state == CHARGING || state == ALERT) {
		moveSpeed = chargeSpeed*timePassed.asSeconds();
	}

	movement.x = moveSpeed*cos(moveAngle);
	movement.y = moveSpeed*sin(moveAngle);

	if(state == MOVING || state == CHARGING) {
		PlayAnim("walk", timePassed);

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
		sprite.setRotation(TO_DEG*moveAngle);
	} else if(state == IDLE) {
		PlayAnim("idle", timePassed);
	} else if(state == ALERT) {
		PlayAnim("alert", timePassed);
		//Rotate the sprite to face the player
		sprite.setRotation(TO_DEG*moveAngle);
	} else {
		// ???
		PlayAnim("idle", timePassed);
	}

	//If the monster was charging and collided into a wall, stop charging
	// Note: Simply hard-stopping the monster produces some unintended behavior
	// when charging down straight hallways; we need to stop him only if he's
	// going slowly in both directions
	if(state == CHARGING && magnitude(movement) <= moveSpeed/2.0f) {
		soundManager.StopSound("charge");
		state = RECOVERING;
		timer = sf::Time::Zero;
	}

	//After updating everything, if the monster is currently charging and hits
	// the player then deal damage
	if(state == CHARGING && IsColliding(level.GetPlayer())) {
		Attack(level.GetPlayer());
	}

	// Decide on a new direction of movement randomly
	if((state == MOVING || state == IDLE) && getRandom() > 0.95f) {
		moveAngle = getRandom()*2*PI;
		//ARGH CONSTANTS
		bool isMoving = getRandom() > 0.90f;
		if(isMoving) {
			state = MOVING;
		} else {
			state = IDLE;
		}
		//printf("New movement: (%g, %g)\n", movement.x, movement.y);
	}
}