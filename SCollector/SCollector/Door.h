#ifndef __DOOR_H
#define __DOOR_H

#include <string>
#include <list>
#include "key.h"
#include "Activatable.h"

class Door : public Activatable
{
public:
	static int Door::CurrentID; 

	Door();
	Door(const sf::Vector2f& pos);
	Door *Instantiate(Key *key, const sf::Vector2f& pos);	//set up Door and its key
	
	void Break();
	void Open(std::list<Key> listKeys);

	Key *GetNewKey(const sf::Vector2f& pos) const;
	double GetHealth() const; 
	bool IsCollidable() const;
	
	virtual void Update(Level& level, const sf::Time& timePassed);
private:
	
	std::string doorName;
	int health;
	bool isOpened;
	bool isBroken;

	int doorID;				// door ID
};

#endif