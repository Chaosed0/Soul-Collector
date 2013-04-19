/**
 * CircleMove.cpp
 *
 * \brief This is a simple SFML program in which you control a circle with the arrow keys.
 * \author Ed Lu
 * \date 1/22/2013
 */

#define MAJOR_VER 1
#define MINOR_VER 0
#define REVIS_VER 0

#include <fstream>

#include "Game.h"
#include "Utility.h"
#include "SFML/System.hpp"

int main(int argc, char* argv[])
{
	//If we're not in debug mode, that means we don't have a console open;
	// redirect stdout to a file
#ifndef HAVE_CONSOLE
	freopen("SCollector.log", "w", stdout);
	freopen("SCollectorError.log", "w", stderr);
#endif

	printf("SCollector version %d:%d:%d\n", MAJOR_VER, MINOR_VER, REVIS_VER);

	initRandom();

	Game game;
	int retval = game.Run();

	//Not normally needed, but if we redirect to a file we might need to flush
	// which close does
#ifndef HAVE_CONSOLE
	fclose(stdout);
	fclose(stderr);
#endif

	return retval;
}