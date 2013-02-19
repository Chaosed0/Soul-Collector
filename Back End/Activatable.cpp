#include "Activatable.h"

Activatable::Activatable()
{
	isActivated = false;
}

void Activatable::activateIt()
{
	isActivated = true;
}

void Activatable::deactivateIt()
{
	isActivated = false;
}
