#include "Door.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"

int Door::CurrentID = 0;

Door::Door()
	: Activatable("Door.png", sf::IntRect(0,0,32,32), sf::IntRect(0,0,32,32))
{
	health = 100.0f;
	isOpened = false;
	isBroken = false;
}

Door::Door(const sf::Vector2f& pos, int doorID)
	: Activatable(" ", sf::IntRect(0,0,32,32), sf::IntRect(0,0,32,32))
{
	health = 100.0;
	isOpened = false;
	isBroken = false;
	SetPos(pos);
}

void Door::Break()
{
	if(this->health == 0)
		isBroken = true;
}

void Door::Open(std::list<Key> listKeys)
{
	std::list<Key>::iterator it = listKeys.begin();

	while (it != listKeys.end() )
	{
		if (it->getKeyID() == this->doorID)
		{
			isOpened = true;
			break;
		}
		else
		{	
			it++;
		}
	}
}

double Door::GetHealth() const
{
	return health;
}

bool Door::IsCollidable() const
{
	if (isOpened && isBroken) {
		return false;
	}
	else if (isOpened && !isBroken) {
		return false;
	}
	else if (!isOpened && isBroken)
	{
		return false;
	}
	else if (!isOpened && !isBroken)
	{
		return true;
	}
}

// this functions to create door and key with IDs
Door *Door::Instantiate(Key *key, const sf::Vector2f& pos)	//Key *key;
{															//Door *door = Door::Instantiate(key);
	Door *newDoor = new Door();

	newDoor->doorID = CurrentID++;

	key = new Key(pos);

	key->setKeyID(newDoor->doorID);

	return newDoor;
}

// this functions to create door and key with IDs
Key *Door::GetNewKey(const sf::Vector2f& pos) const //Door *door = new Door();
{													//Key *key = door->getNewKey();
	Key *key = new Key(pos);

	key->setKeyID(this->doorID);

	return key;
}

void Door::Update(Level& level, const sf::Time& timePassed)
{
	if(IsActive() && !IsFinished()) {
		visible = false;
		if(level.GetPlayer().HasKey(doorID)) {
			Finish();
		} else {
			isActive = false;
		}
	}
}