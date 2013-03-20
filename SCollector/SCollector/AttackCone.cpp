
#include "AttackCone.h"
#include "Level.h"

AttackCone::AttackCone(const sf::Time& time, const sf::Vector2f& origin,
					 float length, float angleBegin, float angleEnd)
{
	this->origin = origin;
	this->length = length;
	this->angleBegin = angleBegin;
	this->angleEnd = angleEnd;

	isExpired = false;
	liveTime = time;
	livedTime = sf::Time::Zero;

	printf("Attack cone: (%g, %g), %g, %g, %g\n", origin.x, origin.y, length, angleBegin, angleEnd);
}

void AttackCone::Update(const Level& level, const sf::Time& timePassed)
{
	livedTime += timePassed;
	if(livedTime > liveTime) {
		isExpired = true;
		printf("Attack cone marked for delete\n");
	}
}

bool AttackCone::Contains(const sf::Vector2f point) const
{
	sf::Vector2f relPoint = origin - point;
	float angleBetween = atan2(relPoint.y, relPoint.x);
	//The point is within the cone if it's within the circle defined
	// by the radius, and within the sector defined by the angles
	return magnitude(relPoint) > length ||
		angleBetween < angleBegin || angleBetween > angleEnd;
}

bool AttackCone::IsExpired() const
{
	return isExpired;
}