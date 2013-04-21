//Implementation of Player class

#include "Player.h"
#include "Level.h"
#include "AttackCone.h"
#include "HUD.h"

const sf::Time Player::attackConeLife = sf::seconds(0.2f);
const sf::Time Player::maxSprintTime = sf::seconds(3.0f);
const sf::Time Player::maxLighterTime = sf::seconds(150.0f);
const sf::Time Player::maxHumanityTime = sf::seconds(300.0f);
const float Player::attackConeLength = 80.0f;
const float Player::attackConeSweep = 2*PI/8;
const float Player::regSpeed = 120.0f;
const float Player::sprintSpeed = 210.0f;
const float Player::humanityDecrease = 0.5f;
const float Player::humanityIncrease = 20.0f;
const int Player::healthIncrease = 50.0f;
const int Player::maxSouls = 3;

Player::Player(const sf::Vector2f& pos)
	: Movable("assets/img/testsheet.png", sf::IntRect(8, 12, 27, 23), sf::IntRect(0, 0, 50, 50))
	, lighter(128, 200, sf::Color(0, 0, 0, 50), pos)
	, ambientLight(128, 700, sf::Color(0, 0, 0, 210), pos)
{
	//When the player starts, he isn't moving anywhere
	moveLeft = moveRight = moveUp = moveDown = false;
	SetPos(pos);

	state = CANMOVE;

	isSprinting = false;

	sprintTimer = maxSprintTime;
	lighterTimer = maxLighterTime;
	humanityTimer = maxHumanityTime;
	souls = 0;

	//Initialize animations (maybe offload some of this data to a text file
	// or something?)
	animManager.AddAnimSet("walk", 1, 0, true);

	//Initialize sounds
	soundManager.AddSound("assets/sound/swoosh.ogg", "swing", false);
	soundManager.AddSound("assets/sound/thwack.ogg", "hit", false);
	soundManager.AddSound("assets/sound/ow.ogg", "got_hit", false);

	lighter.Toggle();
	ambientLight.Toggle();
}

void Player::Reset()
{
	//When the player starts, he isn't moving anywhere
	moveLeft = moveRight = moveUp = moveDown = false;

	isSprinting = false;

	sprintTimer = maxSprintTime;
	lighterTimer = maxLighterTime;
	humanityTimer = maxHumanityTime;
	health = maxHealth;
	souls = 0;
}

void Player::AddLight(Level& level)
{
	level.AddLight(lighter);
	level.AddLight(ambientLight);
}

void Player::StartAttack()
{
	if(state != ATTACKING && state != STARTATTACK) {
		state = STARTATTACK;
	}
}

void Player::Attack(Movable& movable)
{
	if(movable.IsAlive()) {
		Movable::Attack(movable);
		soundManager.PlaySound("hit");
		humanityTimer += std::max(sf::Time::Zero,
			humanityTimer - sf::microseconds((sf::Int64)(maxHumanityTime.asMicroseconds()/humanityDecrease)));
	}
}

void Player::RemoveHealth(int amount)
{
	Movable::RemoveHealth(amount);
	soundManager.PlaySound("got_hit");
}

void Player::MoveLeft(bool start)
{
	moveLeft = start;
}
void Player::MoveRight(bool start)
{
	moveRight = start;
}
void Player::MoveUp(bool start)
{
	moveUp = start;
}
void Player::MoveDown(bool start)
{
	moveDown = start;
}

void Player::Sprint(bool start)
{
	isSprinting = start;
}

void Player::ToggleLighter()
{
	lighter.Toggle();
}

void Player::AddKey(const std::string& doorName)
{
	this->doorNameSet.insert(doorName);
}

bool Player::HasKey(const std::string& doorName)
{
	return (this->doorNameSet).find(doorName) != doorNameSet.end();
}

void Player::AddSoul()
{
	souls++;
	//Whenever the player gets a soul, his stats should increase
	humanityTimer += sf::microseconds((sf::Int64)(maxHumanityTime.asMicroseconds()/humanityIncrease));
	AddHealth(healthIncrease);
}
int Player::GetRemainingSouls()
{
	return maxSouls - souls;
}

void Player::UpdateHud(HUD& hud)
{
	hud.changeFill((int)health, HUD::health);
	hud.changeFill((int)(100*sprintTimer.asMicroseconds()/maxSprintTime.asMicroseconds()), HUD::fatigue);
	hud.changeFill((int)(100*lighterTimer.asMicroseconds()/maxLighterTime.asMicroseconds()), HUD::fuel);
	hud.changeFill((int)(100*humanityTimer.asMicroseconds()/maxHumanityTime.asMicroseconds()), HUD::humanity);
}

void Player::AddHealth(int recovery)
{
	health = std::min(maxHealth, health+recovery);
}

void Player::AddFuel(int fuel)
{
	sf::Time newtime = sf::microseconds(lighterTimer.asMicroseconds()+fuel/100.0f*maxLighterTime.asMicroseconds());
	lighterTimer = std::min(maxLighterTime, newtime);
}

void Player::Update(Level& level, const sf::Time& timePassed)
{
	timer += timePassed;
	bool moving = moveLeft || moveRight || moveUp || moveDown;
	if(state == CANMOVE && moving) {
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
		float moveSpeed;
		sf::Vector2f movement(0, 0);

		if(isSprinting) {
			moveSpeed = sprintSpeed*timePassed.asSeconds();
			sprintTimer = std::max(sf::Time::Zero, sprintTimer-timePassed);
		} else {
			moveSpeed = regSpeed*timePassed.asSeconds();
		}

		if(moveUp) {
			foundTopLeft = level.GetCollide(topLeft, false, false, nearestTopLeft);
			foundBotRight = level.GetCollide(topRight, false, false, nearestBotRight);
			nearest = std::max(nearestTopLeft, nearestBotRight);
			nearest -= (int)top;
			found = foundTopLeft || foundBotRight;
			if(!found)
				nearest = -INT_MAX;
			//printf("Nearest Up: %d\n", nearest);
			movement.y += std::max(-moveSpeed, (float)nearest);
		}
		if(moveDown) {
			foundTopLeft = level.GetCollide(botLeft, false, true, nearestTopLeft);
			foundBotRight = level.GetCollide(botRight, false, true, nearestBotRight);
			nearest = std::min(nearestTopLeft, nearestBotRight);
			nearest -= (int)bot;
			found = foundTopLeft || foundBotRight;
			if(!found)
				nearest = INT_MAX;
			//printf("Nearest Down: %d\n", nearest);
			movement.y += std::min(moveSpeed, (float)nearest);
		}
		if(moveLeft) {
			foundTopLeft = level.GetCollide(topLeft, true, false, nearestTopLeft);
			foundBotRight = level.GetCollide(botLeft, true, false, nearestBotRight);
			nearest = std::max(nearestTopLeft, nearestBotRight);
			nearest -= (int)left;
			found = foundTopLeft || foundBotRight;
			if(!found)
				nearest = -INT_MAX;
			//printf("Nearest Left: %d\n", nearest);
			movement.x += std::max(-moveSpeed, (float)nearest), 0;
		}
		if(moveRight) {
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

		//Correct diagonal movement so that player doesn't move faster
		if(movement.x && movement.y) {
			movement.x /= 1.414f;
			movement.y /= 1.414f;
		}
		sprite.move(movement);

		if(moveRight && moveDown && !moveLeft && !moveUp) sprite.setRotation(45);
		else if(moveLeft && moveDown && !moveRight && !moveUp) sprite.setRotation(135);
		else if(moveLeft && moveUp && !moveRight && !moveDown) sprite.setRotation(225);
		else if(moveRight && moveUp && !moveLeft && !moveDown) sprite.setRotation(315);
		else if(moveRight && !moveLeft) sprite.setRotation(0);
		else if(moveDown && !moveUp) sprite.setRotation(90);
		else if(moveLeft && !moveRight) sprite.setRotation(180);
		else if(moveUp && !moveDown) sprite.setRotation(270);
	} else if(state == ATTACKING) {
		//TODO: Make this the attack animation instead
		PlayAnim("idle", timePassed);
		if(timer >= attackConeLife) {
			state = CANMOVE;
		}
	} else if(state == STARTATTACK) {
		soundManager.PlaySound("swing");

		state = ATTACKING;
		timer = sf::Time::Zero;

		level.AddAttack(AttackCone(attackConeLife, GetPos(), attackConeLength,
			sprite.getRotation()*TO_RAD - attackConeSweep/2.0f,
			sprite.getRotation()*TO_RAD + attackConeSweep/2.0f, *this));

		state = ATTACKING;
	} else {
		//Something's probably wrong
		PlayAnim("idle", timePassed);
	}


	//Regenerate stamina when not running
	if(!isSprinting) {
		sprintTimer = std::min(maxSprintTime, sprintTimer+timePassed);
	}
	if(sprintTimer <= sf::seconds(0.0f)) {
		isSprinting = false;
	}

	//Update the light
	lighter.SetPos(GetPos());
	ambientLight.SetPos(GetPos());
	lighter.Update(level);
	ambientLight.Update(level);
	//If the lighter's on, reduce fuel
	if(lighter.GetIsOn()) {
		lighterTimer = std::max(sf::Time::Zero, lighterTimer-timePassed);
		if(lighterTimer <= sf::Time::Zero) {
			lighter.Toggle();
		}
	}
	//Reduce time we have
	humanityTimer = std::max(sf::Time::Zero, humanityTimer-timePassed);
}