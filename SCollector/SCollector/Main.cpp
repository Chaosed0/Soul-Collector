/**
 * CircleMove.cpp
 *
 * \brief This is a simple SFML program in which you control a circle with the arrow keys.
 * \author Ed Lu
 * \date 1/22/2013
 */

#include "Game.h"
#include "Utility.h"

int main(int argc, char* argv[])
{
	initRandom();

	Game game;
	return game.Run();
}