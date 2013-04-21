
#include "AttackCone.h"
#include "Level.h"
#include "Movable.h"

AttackCone::AttackCone(const sf::Time& time, const sf::Vector2f& origin,
					 float length, float angleBegin, float angleEnd, Movable& owner)
					 : owner(owner)
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

	//printf("Attack cone: (%g, %g), %g, %g, %g\n", origin.x, origin.y, length, this->angleBegin, this->angleEnd);
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
	float corAngleBegin = angleBegin;
	float corAngleEnd = angleEnd;
	if(angleBegin > angleEnd && angleBetween > angleBegin) {
		corAngleEnd += 2*PI;
	} else if(angleBegin > angleEnd && angleBetween < angleEnd) {
		corAngleBegin-= 2*PI;
	}
	//The point is within the cone if it's within the circle defined
	// by the radius, and within the sector defined by the angles
	//printf("%g vs %g, %g vs (%g, %g)\n", magnitude(relPoint), length, angleBetween, angleBegin, angleEnd);
	return magnitude(relPoint) < length &&
		angleBetween > corAngleBegin && angleBetween < corAngleEnd;
}

void AttackCone::MovableHit(const Movable& movable)
{
	movablesHit.insert(&movable);
}

bool AttackCone::IsMovableHit(const Movable& movable) const
{
	return movablesHit.find(&movable) != movablesHit.end();
}

bool AttackCone::IsExpired() const
{
	return isExpired;
}
Movable& AttackCone::GetOwner() const
{
	return owner;
}