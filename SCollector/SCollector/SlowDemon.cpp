
#include "SlowDemon.h"
#include "Level.h"
#include "Player.h"

const sf::Time SlowDemon::attackTime = sf::seconds(1.0f);
const float SlowDemon::moveSpeed = 100.0f;

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
}

void SlowDemon::Attack(Movable& movable)
{
}

void SlowDemon::Update(Level& level, const sf::Time& timePassed)
{
}