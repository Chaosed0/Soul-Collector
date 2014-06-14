
#include "SlowDemon.h"
#include "Level.h"
#include "Player.h"

const sf::Time SlowDemon::attackTime = sf::seconds(1.0f);
const float SlowDemon::defaultMoveSpeed = 50.0f;
const float SlowDemon::attackRadius = 25.0f;
const float SlowDemon::attackSweep = 45.0f;

SlowDemon::SlowDemon(sf::Vector2f pos)
	: Movable("assets/img/zombie_topdown.png", sf::IntRect(20, 20, 27, 23), sf::IntRect(0, 0, 64, 64))
{
	sprite.setPosition(pos);
	state = IDLE;

	timer = sf::Time::Zero;

	animManager.ModifyAnimSet("idle", 0, 0, true);
	animManager.AddAnimSet("alert", 0, 3, true);
	animManager.AddAnimSet("walk", 4, 11, true);
	animManager.AddAnimSet("attack", 13, 18, true);
	animManager.AddAnimSet("death", 21, 27, false);

	soundManager.AddSound("assets/sound/charge.ogg", "charge", true);
	soundManager.AddSound("assets/sound/alert.ogg", "alert", false);
	soundManager.AddSound("assets/sound/thwack.ogg", "hit", false);

	attackPower = 5;
}

AttackCone SlowDemon::GetAttackCone()
{
	//I have no idea why I need to add PI to the attack angles here, to be honest
	return AttackCone(attackTime, GetPos(), attackRadius,
		sprite.getRotation()*TO_RAD - attackSweep/2.0f + PI,
		sprite.getRotation()*TO_RAD + attackSweep/2.0f + PI, *this);
}

void SlowDemon::Update(Level& level, const sf::Time& timePassed)
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
	level.GetCollide(GetPos(), playerPos, nearest);
	float distFromWall = magnitude(GetPos()-nearest);
	bool canSeePlayer = dist <= distFromWall;
	//printf("player:(%g,%g), nearest:(%g,%g), angle: %g\n", playerPos.x, playerPos.y, nearest.x, nearest.y, atan2(relDist.y, relDist.x)*TO_DEG);
	//printf("dist:%g, nearest:%g, cansee:%d\n", dist, distFromWall, canSeePlayer);
	float moveSpeed = defaultMoveSpeed*timePassed.asSeconds();
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
	
	if(state == ATTACKING && timer > attackTime) {
		//If the demon's attacked for long enough, stop attacking 
		state = IDLE;
		//printf("Demon attack timer expired\n");
		timer = sf::Time::Zero;
	}

	if((state == IDLE || state == MOVING) && canSeePlayer && dist < SPOT_RADIUS) {
		//If the player is close to the demon, put on alert
		state = ALERT;
		soundManager.PlaySound("alert");
		//printf("Demon is alerted\n");
		timer = sf::Time::Zero;
	} else if(state == ALERT && (dist >= SPOT_RADIUS || !canSeePlayer)) {
		//If the player moved out of the monster's visibility range, then go back to
		// idling
		state = IDLE;
		//printf("Demon is no longer alerted\n");
		timer = sf::Time::Zero;
	} else if(state == ALERT && dist <= attackRadius) {
		level.AddAttack(GetAttackCone());
		state = ATTACKING;
		timer = sf::Time::Zero;
	} else if(state == ALERT) {
		//If the demon is alerted, follow the player around
		moveAngle = atan2f(relDist.y, relDist.x);
		//printf("Demon is charging player after %g seconds\n", timer.asSeconds());
		timer = sf::Time::Zero;
	}

	movement.x = moveSpeed*cos(moveAngle);
	movement.y = moveSpeed*sin(moveAngle);

	if(state == MOVING || state == ALERT) {
		PlayAnim("walk", timePassed);

		move(level, movement);
		sprite.setRotation(TO_DEG*moveAngle);
	} else if(state == IDLE) {
		PlayAnim("idle", timePassed);
	} else if(state == ALERT) {
		PlayAnim("alert", timePassed);
		//Rotate the sprite to face the player
		sprite.setRotation(TO_DEG*moveAngle);
	} else if(state == ATTACKING) {
		PlayAnim("attack", timePassed);
	} else {
		// ???
		PlayAnim("idle", timePassed);
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
