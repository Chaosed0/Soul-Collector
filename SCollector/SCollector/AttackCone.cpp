
#include "AttackCone.h"
#include "Level.h"
#include "Movable.h"

AttackCone::AttackCone(const sf::Time& time, const sf::Vector2f& origin,
					 float length, float angleBegin, float angleEnd)
{
	this->origin = origin;
	this->length = length;
	this->angleBegin = angleBegin;
	this->angleEnd = angleEnd;

	//Make the angles between -PI and PI 
	this->angleBegin = shiftAngle(angleBegin);
	this->angleEnd = shiftAngle(angleEnd);

	isExpired = false;
	liveTime = time;
	livedTime = sf::Time::Zero;

	//printf("Attack cone: (%g, %g), %g, %g, %g\n", origin.x, origin.y, length, angleBegin, angleEnd);
}

void AttackCone::Update(const Level& level, const sf::Time& timePassed)
{
	livedTime += timePassed;
	if(livedTime > liveTime) {
		isExpired = true;
		//printf("Attack cone marked for delete\n");
	}
}

bool AttackCone::Contains(const sf::Vector2f point) const
{
	sf::Vector2f relPoint = point - origin;
	float angleBetween = atan2(relPoint.y, relPoint.x);
	angleBetween = shiftAngle(angleBetween);
	//The point is within the cone if it's within the circle defined
	// by the radius, and within the sector defined by the angles
	printf("%g vs %g, %g vs (%g, %g)\n", magnitude(relPoint), length, angleBetween, angleBegin, angleEnd);
	return magnitude(relPoint) < length &&
		angleBetween > angleBegin && angleBetween < angleEnd;
}

void AttackCone::MovableHit(const Movable& movable)
{
	movablesHit.push_back(&movable);
}

bool AttackCone::IsMovableHit(const Movable& movable) const
{
	for(unsigned int i = 0; i < movablesHit.size(); i++) {
		if(movablesHit[i] == &movable) {
			return true;
		}
	}
	return false;
}

bool AttackCone::IsExpired() const
{
	return isExpired;
}