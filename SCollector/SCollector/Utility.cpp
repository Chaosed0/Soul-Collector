
#include "Utility.h"

std::mt19937 randgen;

float magnitude(const sf::Vector2f& vec)
{
	return sqrt(vec.x*vec.x+vec.y*vec.y);
}

sf::Vector2f normalize(const sf::Vector2f &vec) {
	return vec / magnitude(vec);
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

int getQuadrant(const sf::Vector2f &p1, const sf::Vector2f &p2) {
	if(p2.x > p1.x && p2.y > p1.y) {
		return 1;
	} else if(p2.x < p1.x && p2.y > p1.y) {
		return 2;
	} else if(p2.x < p1.x && p2.y < p1.y) {
		return 3;
	} else if(p2.x > p1.x && p2.y < p1.y) {
		return 4;
	} else {
		// ???
		return -1;
	}
}

sf::Vector2f getQuadrantVec(int quadrant, float magnitude) {
	switch(quadrant) {
		case 1:
			return sf::Vector2f(magnitude,magnitude);
			break;
		case 2:
			return sf::Vector2f(-magnitude,magnitude);
			break;
		case 3:
			return sf::Vector2f(-magnitude,-magnitude);
			break;
		case 4:
			return sf::Vector2f(magnitude,-magnitude);
			break;
		default:
			return sf::Vector2f(0,0);
			break;
	}
	return sf::Vector2f(0,0);
}

sf::Vector2f lineIntersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4) {
	return sf::Vector2f(
			((p1.x*p2.y - p1.y*p2.x) * (p3.x - p4.x) - (p1.x - p2.x) * (p3.x*p4.y - p3.y*p4.x)) /
			((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x)),
			((p1.x*p2.y - p1.y*p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x*p4.y - p3.y*p4.x)) /
			((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x)));
}
