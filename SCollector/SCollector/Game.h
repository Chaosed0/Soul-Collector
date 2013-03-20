/**
 * \brief Main game class.
 * \author Edward Lu
 * \date February 2 2013
 */

#ifndef __GAME_H
#define __GAME_H

#include <SFML/Graphics.hpp>

#include "MenuManager.h"
#include "Level.h"
#include "HUD.h"

class Game
{
public:
	/** Default constructor. */
	Game();
	/** Runs the game. */
	int Run();
private:
	/** Initialization */
	void Init();
	/** Main game loop - calls Event(), Update(), and Render() */
	void Loop();
	/** Processes events from the RenderWindow */
	void Event();
	/** Updates the game state per frame */
	void Update();
	/** Renders the game objects to the screen */
	void Render();

	/** Exits the game */
	void Exit();

	/** Width and height of the window */
	const int winWidth, winHeight;
	/** The window itself */
	sf::RenderWindow window;
	/** The menu manger */
	MenuManager menus;

	/** Flag set to true when the game is running */
	bool isRunning;

	/** The level object */
	Level level;

	/** The camera */
	sf::View view;

	/** Timer to pass to Level::Update */
	sf::Clock updateTimer;
};

#endif
