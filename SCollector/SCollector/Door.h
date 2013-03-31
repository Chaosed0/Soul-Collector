#ifndef __DOOR_H
#define __DOOR_H

#include <string>
#include "key.h"

class Door
{
public:
	static Door *Instantiate(Key *key, const sf::Vector2f& pos);	//set up Door and its key

	static int CurrentID;
	
	void Break();

	Door();

	Key *getNewKey(const sf::Vector2f& pos);


private:
	std::string doorName;
	int health;
	bool isOpened;
	bool isBroken;

	int ID;				// door ID
};

#endif