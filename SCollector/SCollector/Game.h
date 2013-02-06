/**
 * \brief Main game class.
 * \author Edward Lu
 * \date February 2 2013
 */

#include <SFML/Graphics.hpp>

#include "MenuManager.h"
#include "Player.h"

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
	bool running;

	Player player;
};