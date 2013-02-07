//SFML mian template
//Edward Lu

#include "Player.h"
#include "Enemy.h"
#include <time.h>
#include <SFML/Graphics.hpp>

#define FRAMES_PER_SEC 60.0

int main(int argc, char* argv[])
{
	bool moveUp, moveDown, moveLeft, moveRight;
	moveUp = moveDown = moveLeft = moveRight = false;
	int windowWidth = 1000;
	int windowHeight = 750;
	float playerMoveSpeed = .25;
	float enemyMoveSpeed = .25;
	std::string playerImage = "square.jpg";
	std::string enemyImage = "circle.png";
	Player player(playerImage,windowWidth,windowHeight,playerMoveSpeed);
	srand(time(0));
	Enemy enemy1(enemyImage,windowWidth,windowHeight,enemyMoveSpeed);
	Enemy enemy2(enemyImage,windowWidth,windowHeight,enemyMoveSpeed);

	//Main window
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Main Window");

	//Flag set to true when the program should stop running
	bool running = true;

	//Frame limiter clock
	sf::Clock frameLimiter;
	//Time for a frame
	sf::Time frameTime = sf::microseconds(1.0/FRAMES_PER_SEC);

	window.setKeyRepeatEnabled(false);

	//Main loop
	while(running)
	{
		frameLimiter.restart();

		//Event handling
		sf::Event winEvent;
		while(window.pollEvent(winEvent))
		{
			switch(winEvent.type)
			{
			case sf::Event::KeyPressed :
				/* KEYBOARD CODE */
				switch(winEvent.key.code)
				{
				case sf::Keyboard::Escape :
					running = false;
					break;
				case sf::Keyboard::W:
					moveUp = true;
					break;
				case sf::Keyboard::A:
					moveLeft = true;
					break;
				case sf::Keyboard::S:
					moveDown = true;
					break;
				case sf::Keyboard::D:
					moveRight = true;
					break;
				default:
					//Just here to stop warnings
					break;
				}
				break;
				
			case sf::Event::KeyReleased:
				switch(winEvent.key.code)
				{
				case sf::Keyboard::W:
					moveUp = false;
					break;
				case sf::Keyboard::A:
					moveLeft = false;
					break;
				case sf::Keyboard::S:
					moveDown = false;
					break;
				case sf::Keyboard::D:
					moveRight = false;
					break;
				}
				break;

			case sf::Event::Closed :
				running = false;
				break;
			default:
				//Just here to stop warnings
				break;
			}

			
		}

		if (player.collisionCheck(enemy1) || player.collisionCheck(enemy2))
			printf("Collision detected.\n");
		
		player.move(moveUp, moveLeft, moveDown, moveRight, windowWidth, windowHeight);
		enemy1.autoRun(windowWidth, windowHeight);
		enemy2.autoRun(windowWidth, windowHeight);

		window.clear();

		window.draw(player);
		window.draw(enemy1);
		window.draw(enemy2);

		//Render things using window.draw()
		window.display();

		//Limit framerate
		if(frameLimiter.getElapsedTime() < frameTime)
			sf::sleep(sf::microseconds(frameLimiter.getElapsedTime().asMicroseconds() - frameTime.asMicroseconds()));
		else
			printf("slow\n");

	}

	//End
	return 0;

}