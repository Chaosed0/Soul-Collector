#ifndef __DOOR_H
#define __DOOR_H

#include <string>
#include <list>
#include "key.h"
#include "Activatable.h"

class Door : public Activatable
{
public:
	Door();
	Door(const sf::Vector2f& pos);

	Door(const sf::Vector2f& pos, std::string doorName);
	
	void Break();

	double GetHealth() const; 
	bool IsCollidable() const;
	
	virtual void Update(Level& level, const sf::Time& timePassed);
private:
	std::string doorName;
	int health;
	bool isOpened;
	bool isBroken;	
};

#endif