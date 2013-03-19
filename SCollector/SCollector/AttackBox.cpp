
#include "AttackBox.h"
#include "Level.h"

AttackBox::AttackBox(const sf::Time& time, const sf::IntRect& box)
	: sf::IntRect(box)
{
	isExpired = false;
	liveTime = time;
}

void AttackBox::Update(const Level& level)
{
	if(timer.getElapsedTime().asMicroseconds() > liveTime.asMicroseconds()) {
		isExpired = true;
	}
}