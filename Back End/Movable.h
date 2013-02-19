#ifndef MOVABLE_H
#define MOVABLE_H

#include "Drawable.h"

class Movable: public Drawable
{
private:
	double currentSpeed;
	double attackPower;
	double currentHP;
	
public:
	Movable();

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void attack();
	
	double getCurrentSpeed();
	double getAttackPower();
	double getCurrentHP();
	
	void setCurrentSpeed(double speed);
	void setAttackPower(double power);
	void setCurrentHP(double HP);
}


#endif