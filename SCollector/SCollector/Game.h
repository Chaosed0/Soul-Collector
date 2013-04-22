/**
 * \brief Main game class.
 * \author Edward Lu
 * \date February 2 2013
 */

#ifndef __GAME_H
#define __GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "MenuManager.h"
#include "LevelManager.h"
#include "HUD.h"
#include "SoundManager.h"

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
	/** Renders the intro to the screen. */
	void Intro();

	/** Exits the game */
	void Exit();

	/** Width and height of the window */
	const int winWidth, winHeight;
	/** The window itself */
	sf::RenderWindow window;
	/** The menu manger */
	MenuManager menus;
	/** Sound manager for playing sounds at, for example, game over */
	SoundManager soundManager;

	/** Flag set to true when the game is running */
	bool isRunning;

	/** The level manager object */
	LevelManager levelManager;

	/** The camera */
	sf::View view;

	/** Timer to pass to Level::Update */
	sf::Clock updateTimer;

	/** HUD object */
	HUD hud;

	/** Fade out timer for win/loss. */
	sf::Time fadeoutTimer;
	/** Fade out rectangle covering the entire view */
	sf::RectangleShape fadeoutRect;
	/** Flag to pause the game while fading out */
	bool paused;

	/** Timer for the first play trigger */
	sf::Time shakeTimer;
	/** Flag for the shake timer */
	bool shakeDone;

	/** How much time we should fade out for. */
	const static sf::Time fadeoutTime;
	/** How much time the intro should be displayed. */
	const static sf::Time introTime;
	/** How much time to shake the screen initially. */
	const static sf::Time shakeTime;
	/** The first level to load */
	const static std::string firstLevel;
	const static std::string firstSpawn;
};

#endif
