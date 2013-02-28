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

	const int winWidth, winHeight;
	MenuManager menus;
	sf::RenderWindow window;
	bool isRunning;

	Level level;

	sf::View view;

	//TEST
	sf::RenderTexture overlayTexture;
	sf::Sprite overlaySprite;
	sf::Texture circleTexture;
	sf::Sprite circle;
	sf::Sprite circle2;
	sf::RectangleShape rect;
};

#endif