#include "door.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

int Door::CurrentID = 0;

void Door::Break()
{
}

Door::Door()
{
}

// this functions to create door and key with IDs
Door *Door::Instantiate(Key *key, const sf::Vector2f& pos)	//Key *key;
{															//Door *door = Door::Instantiate(key);
	Door *newDoor = new Door();

	newDoor->ID = CurrentID++;

	key = new Key(pos);

	key->setKeyID(newDoor->ID);

	return newDoor;
}

// this functions to create door and key with IDs
Key *Door::getNewKey(const sf::Vector2f& pos)		//Door *door = new Door();
{													//Key *key = door->getNewKey();
	Key *key = new Key(pos);

	key->setKeyID(this->ID);

	return key;
}