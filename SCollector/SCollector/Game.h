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
#include "LightOverlay.h"

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

	const int winWidth, winHeight;
	sf::RenderWindow window;
	MenuManager menus;
	bool isRunning;

	Level level;

	//TEST
	sf::View view;
	LightOverlay playerLight;
	sf::RenderTexture lightOverlay;
	sf::Sprite lightOverlaySprite;
};

#endif
