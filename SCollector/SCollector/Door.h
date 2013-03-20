#ifndef __DOOR_H
#define __DOOR_H

#include <string>

class Door
{
public:
	void Break();

private:
	std::string doorName;
	int health;
	bool isOpened;
	bool isBroken;
};

#endif