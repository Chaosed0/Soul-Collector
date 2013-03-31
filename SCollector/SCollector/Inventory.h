#include <list>
#include <string>
using namespace std;

#pragma once

class Inventory
{
public:
	//Inventory(void);
	~Inventory(void);
	void Additem (list<string>);
};
list<string> Inventory;
