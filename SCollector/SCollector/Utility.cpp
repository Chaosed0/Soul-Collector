
#include "Utility.h"

float magnitude(const sf::Vector2f& vec)
{
	return sqrt(vec.x*vec.x+vec.y*vec.y);
}

float shiftAngle(float angle)
{
	while(angle < -PI || angle > PI) {
		if(angle < -PI) { 
			angle += 2*PI;
		} else if(angle > PI) {
			angle -= 2*PI;
		}
	}

	return angle;
}