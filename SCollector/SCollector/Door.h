#ifndef __DOOR_H
#define __DOOR_H

#include <string>
#include "Key.h"
#include "Activatable.h"


class Door : public Activatable
{
public:
	Door(const sf::Vector2f& pos, const std::string& doorName);
	
	void Break();

	double GetHealth() const; 
	
	virtual void Update(Level& level, const sf::Time& timePassed);
private:
	std::string doorName;
	int health;
	bool isOpened;
	bool isBroken;	
};

#endif