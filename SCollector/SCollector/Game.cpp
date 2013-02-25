
#include "Game.h"

Game::Game() :
	winWidth(800), winHeight(600),
	window(sf::VideoMode(winWidth, winHeight), "Test") ,
	menus(winWidth, winHeight),
	player(sf::Vector2f(winWidth/2.0f, winHeight/2.0f)) ,
	level("new.tmx") ,
	rect(sf::Vector2f(winWidth, winHeight))
{
	//Flag set to false when the game ends
	isRunning = false;

	circleTexture.loadFromFile("assets/img/LightAura.png");
	circle.setTexture(circleTexture);
	circle.setOrigin(circle.getTextureRect().width/2, circle.getTextureRect().height/2);
	rect.setFillColor(sf::Color());
	overlay.create(window.getSize().x, window.getSize().y);
}

int Game::Run()
{
	isRunning = true;
	Init();
	Loop();

	return 0;
}

void Game::Init()
{
	// --- INITIALIZATION ---
	//Turn off repeating keypresses (because there's a delay between pressing the key and
	// perceiving it as held down)
	window.setKeyRepeatEnabled(false);

	//Connect the exit button of the menus to the exit function here
	menus.connectExitFunc(&Game::Exit, this);

	//Set the position of the player to the level spawn coords
	player.SetPos(level.GetSpawn());
}

void Game::Loop()
{
	//Timer for regulating framerate - for an explanation of this, go to the end of
	// the main game loop
	sf::Clock timer;
	//How many frames per second we want the game to run
	const int ticksPerSec = 60;
	//The time needed for that frame
	sf::Time frameTime = sf::microseconds(sf::seconds(1).asMicroseconds()/ticksPerSec);

	// --- MAIN GAME LOOP ---
	while(isRunning)
	{
		//Reset the framerate regulation timer
		timer.restart();

		Event();
		//Only update the game if the gui is not currently shown
		if(!menus.IsVisible())
			Update();
		Render();

		// --- FRAME CORRECTION ---
		//The theory behind this bit is that with faster and faster hardware, the functions and
		// such in this for loop will run faster and faster. We don't want that to happen, since
		// the movement code is here - the player speed shouldn't depend on the hardware! So, we
		// force the game to wait for a bit if we processed the loop too fast.
		//The SFML time functions are a bit unwieldy to use, but fairly easy to follow
		sf::Time timeSleep = sf::microseconds(frameTime.asMicroseconds() - timer.getElapsedTime().asMicroseconds());
		if(timeSleep.asMicroseconds() >= 0)
			sf::sleep(timeSleep);
		//If the time to sleep is less than 0, we don't have to do anything, though the game will
		// start to lag
	}
}

void Game::Event()
{
	// --- EVENT PROCESSING ---
	// Why is "event" a keyword in Visual Studio? We may never know...
	sf::Event anEvent;
	while(window.pollEvent(anEvent)) {
		//This one needs to go before the menu receives the input, so that 
		// the window can still close if the menu's open
		if(anEvent.type == sf::Event::Closed) {
			Exit();
		}
		//If the menu system isn't visible, then it can't receive events
		else if(menus.IsVisible()) {
			menus.HandleEvent(anEvent);
		}
		//Otherwise, the game is visible, and it should receive events
		// Note: May change later!
		else {
			switch(anEvent.type) {
			case sf::Event::KeyPressed :
				switch(anEvent.key.code) {
				//Note: The reason I don't move the player shape within these case statements is twofold:
				// 1.) Separation of getting input and actually performing the action is VERY important
				//  (even though it was hella annoying in Hogan's class)
				// 2.) This event is for a key PRESS, so the player will only move once even if the
				//  key is held down! Yes, there is a property on the window that allows keys to repeat
				//  their presses if held down that I turned off earlier, but I did that because there's
				//  a delay between getting the press and actually determining that the key is held down.
				case sf::Keyboard::Up :
					player.MoveUp(true);
					break;
				case sf::Keyboard::Down :
					player.MoveDown(true);
					break;
				case sf::Keyboard::Left :
					player.MoveLeft(true);
					break;
				case sf::Keyboard::Right :
					player.MoveRight(true);
					break;
				case sf::Keyboard::Escape :
					menus.SetVisible(true);
					break;
				}
				break;
			case sf::Event::KeyReleased :
				switch(anEvent.key.code) {
				case sf::Keyboard::Up :
					player.MoveUp(false);
					break;
				case sf::Keyboard::Down :
					player.MoveDown(false);
					break;
				case sf::Keyboard::Left :
					player.MoveLeft(false);
					break;
				case sf::Keyboard::Right :
					player.MoveRight(false);
					break;
				}
				break;
			}
		}
	}
}

void Game::Update()
{
	// --- GAME LOGIC ---
	//Update the player class
	player.Update(level);

	circle.setPosition(sf::Vector2f(player.GetPos().x, winHeight-player.GetPos().y));
}

void Game::Render()
{
	// --- RENDERING ---
	//Clear the screen of what was drawn before
	// This is inefficient, but if we don't call it, then there will be artefacts from the
	// previous frame
	//Note: Since we're rendering the level and that takes up all of the screen, we don't
	// need to clear the screen
	//window.clear(sf::Color(255, 255, 255, 255));
	//Render the level to the screen, before the player
	window.draw(level);
	//Render the player to the screen
	window.draw(player);
	overlay.draw(rect);
	overlay.draw(circle, sf::RenderStates(sf::BlendMode::BlendNone));
	window.draw(sf::Sprite(overlay.getTexture()));

	menus.Display(window);

	//Update the window after the drawing
	window.display();
}

void Game::Exit()
{
	isRunning = false;
}