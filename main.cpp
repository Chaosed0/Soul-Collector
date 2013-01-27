//SFML mian template
//Edward Lu

#include <SFML/Graphics.hpp>

#define FRAMES_PER_SEC 60.0

int main(int argc, char* argv[])
{
	//Declarations go here...

	//Main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Main Window");

	//Flag set to true when the program should stop running
	bool running = true;

	//Frame limiter clock
	sf::Clock frameLimiter;
	//Time for a frame
	sf::Time frameTime = sf::microseconds(1.0/FRAMES_PER_SEC);

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
				default:
					//Just here to stop warnings
					break;
				}
				break;
				/* KEYBOARD CODE */
			case sf::Event::Closed :
				running = false;
				break;
			default:
				//Just here to stop warnings
				break;
			}
		}

		//Render things using window.draw()
		window.display();

		//Limit framerate
		if(frameLimiter.getElapsedTime() < frameTime)
			sf::sleep(sf::microseconds(frameLimiter.getElapsedTime().asMicroseconds() - frameTime.asMicroseconds()));
	}

	//End
	return 0;
}