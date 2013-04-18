
#include "Utility.h"

std::mt19937 randgen;

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

void initRandom()
{
	randgen.seed((unsigned long)time(NULL));
}

float getRandom()
{
	return (float)randgen()/(float)randgen.max();
}

float getRandom(float begin, float end)
{
	return getRandom()*(end-begin)+begin;
}

bool IsValidProperty(const std::string& prop)
{
	return !prop.empty() && prop.compare("No such property!") != 0;
}