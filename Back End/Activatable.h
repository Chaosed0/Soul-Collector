#ifndef ACTIVATABLE_H
#define ACTIVATABLE_H

#include "Drawable.h"

class Activatable: public Drawable
{
private:
	bool isActivated;

public:
	Activatable();
	
	void activateIt();
	void deactivateIt();
}


#endif