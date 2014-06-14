//Implementation of Player class

#include "Player.h"
#include "Level.h"
#include "AttackCone.h"
#include "HUD.h"

const sf::Time Player::attackConeLife = sf::seconds(0.2f);
const sf::Time Player::maxSprintTime = sf::seconds(3.0f);
const sf::Time Player::maxLighterTime = sf::seconds(300.0f);
const sf::Time Player::maxHumanityTime = sf::seconds(600.0f);
const float Player::attackConeLength = 50.0f;
const float Player::attackConeSweep = 2*PI/8;
const float Player::regSpeed = 120.0f;
const float Player::sprintSpeed = 200.0f;
const float Player::humanityDecrease = 0.2f;
const float Player::humanityIncrease = 80.0f;
const int Player::healthIncrease = 50;
const int Player::maxSouls = 7;
const int Player::baseAttack = 20;

Player::Player(const sf::Vector2f& pos)
	: Movable("assets/img/Player.png", sf::IntRect(8, 12, 27, 23), sf::IntRect(0, 0, 50, 50))
	, lighter(200, sf::Color(0, 0, 0, 50), pos)
	, ambientLight(700, sf::Color(0, 0, 0, 210), pos)
{
	//When the player starts, he isn't moving anywhere
	SetPos(pos);

	state = CANMOVE;

	isSprinting = false;

	sprintTimer = maxSprintTime;
	lighterTimer = maxLighterTime;
	humanityTimer = maxHumanityTime;
	souls = 0;

	//Initialize animations (maybe offload some of this data to a text file
	// or something?)
	animManager.AddAnimSet("walk", 1, 16, true);
	animManager.AddAnimSet("attack", 17, 23, false);

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
	dir = sf::Vector2f(0,0);

	isSprinting = false;

	sprintTimer = maxSprintTime;
	lighterTimer = maxLighterTime;
	humanityTimer = maxHumanityTime;
	health = maxHealth;
	souls = 0;
	doorNameSet.clear();
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

bool Player::IsAlive()
{
	return Movable::IsAlive() && humanityTimer != sf::Time::Zero;
}

void Player::RemoveHealth(int amount)
{
	Movable::RemoveHealth(amount);
	soundManager.PlaySound("got_hit");
}

void Player::MoveLeft(bool start)
{
	dir.x -= (start ? 1.0f : -1.0f);
}
void Player::MoveRight(bool start)
{
	dir.x += (start ? 1.0f : -1.0f);
}
void Player::MoveUp(bool start)
{
	dir.y -= (start ? 1.0f : -1.0f);
}
void Player::MoveDown(bool start)
{
	dir.y += (start ? 1.0f : -1.0f);
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
	AddHumanity(humanityIncrease);
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

void Player::AddEnergy(int recovery)
{
	sf::Time newtime = sf::microseconds((sf::Int64)
		(sprintTimer.asMicroseconds()+recovery/100.0f*maxSprintTime.asMicroseconds()));
	sprintTimer = std::min(maxSprintTime, newtime);
}

void Player::AddFuel(int fuel)
{
	sf::Time newtime = sf::microseconds((sf::Int64)
		(lighterTimer.asMicroseconds()+fuel/100.0f*maxLighterTime.asMicroseconds()));
	lighterTimer = std::min(maxLighterTime, newtime);
}

void Player::AddHumanity(int humanity)
{
	sf::Time newtime = sf::microseconds((sf::Int64)
		(humanityTimer.asMicroseconds()+humanity/100.0f*maxHumanityTime.asMicroseconds()));
	humanityTimer = std::min(maxHumanityTime, newtime);
}

void Player::Update(Level& level, const sf::Time& timePassed)
{
	timer += timePassed;
	bool moving = dir.x != 0 || dir.y != 0;
	float moveSpeed;
	sf::Vector2f movement;

	attackPower = baseAttack + (100-baseAttack)*(1-humanityTimer.asMicroseconds()/(float)maxHumanityTime.asMicroseconds());

	if(isSprinting) {
		moveSpeed = sprintSpeed*timePassed.asSeconds();
		sprintTimer = std::max(sf::Time::Zero, sprintTimer-timePassed);
	} else {
		moveSpeed = regSpeed*timePassed.asSeconds();
	}

	movement.x = dir.x == 0 ? 0 : dir.x * moveSpeed / magnitude(dir);
	movement.y = dir.y == 0 ? 0 : dir.y * moveSpeed / magnitude(dir);

	if(state == CANMOVE && moving) {
		PlayAnim("walk", timePassed);
		move(level, movement);
		sprite.setRotation(TO_DEG * atan2f(movement.y, movement.x));
	} else if(state == ATTACKING) {
		PlayAnim("attack", timePassed);
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
