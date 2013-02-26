
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "MenuManager.h"

Game::Game() :
	winWidth(800), winHeight(600),
	window(sf::VideoMode(winWidth, winHeight), "Test") ,
	menus(winWidth, winHeight),
<<<<<<< HEAD
	player(sf::Vector2f(winWidth/2.0f, winHeight/2.0f), "assets/img/what.png"),
	level("test2.tmx")
=======
	level("new.tmx") ,
	rect(sf::Vector2f((float)winWidth, (float)winHeight)) ,
	view(sf::FloatRect(0.0f, 0.0f, (float)winWidth, (float)winHeight))
>>>>>>> 52e2cb23946dd203c36796780aaa091a78d1664b
{
	//Flag set to false when the game ends
	isRunning = false;

	circleTexture.loadFromFile("assets/img/LightAura.png");
	circle.setTexture(circleTexture);
	circle.setOrigin(circle.getTextureRect().width/2.0f, circle.getTextureRect().height/2.0f);
	rect.setFillColor(sf::Color(0,0,0,255));
	overlayTexture.create(window.getSize().x, window.getSize().y);
	overlaySprite.setTexture(overlayTexture.getTexture());
	overlaySprite.setOrigin(overlayTexture.getSize().x/2.0f, overlayTexture.getSize().y/2.0f);

	window.setView(view);
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
					level.GetPlayer().MoveUp(true);
					break;
				case sf::Keyboard::Down :
					level.GetPlayer().MoveDown(true);
					break;
				case sf::Keyboard::Left :
					level.GetPlayer().MoveLeft(true);
					break;
				case sf::Keyboard::Right :
					level.GetPlayer().MoveRight(true);
					break;
				case sf::Keyboard::Escape :
					menus.SetVisible(true);
					break;
				}
				break;
			case sf::Event::KeyReleased :
				switch(anEvent.key.code) {
				case sf::Keyboard::Up :
					level.GetPlayer().MoveUp(false);
					break;
				case sf::Keyboard::Down :
					level.GetPlayer().MoveDown(false);
					break;
				case sf::Keyboard::Left :
					level.GetPlayer().MoveLeft(false);
					break;
				case sf::Keyboard::Right :
					level.GetPlayer().MoveRight(false);
					break;
				}
				break;
			}
		}
	}
}

void Game::Update()
{
	sf::Vector2f playerPos = level.GetPlayer().GetPos();
	sf::Vector2f viewSize = view.getSize();
	sf::Vector2f viewPos = view.getCenter();
	sf::Vector2i levelSize = level.GetSize();

	// --- GAME LOGIC ---
	//Update the level
	level.Update();

	//printf("player: (%g, %g) view: (%g, %g)\n", playerPos.x, playerPos.y, viewPos.x, viewPos.y);

	//Update the view to follow the player, but don't make it go offscreen
	if(playerPos.x - viewSize.x/2 >= 0 && playerPos.x + viewSize.x/2 <= levelSize.x)
		view.setCenter(sf::Vector2f(playerPos.x, viewPos.y));
	else if(playerPos.x - viewSize.x/2 < 0)
		view.setCenter(sf::Vector2f(viewSize.x/2, viewPos.y));
	else if(playerPos.x + viewSize.x/2 > levelSize.x)
		view.setCenter(sf::Vector2f(levelSize.x - viewSize.x/2, viewPos.y));

	viewPos = view.getCenter();

	if(playerPos.y - viewSize.y/2 >= 0 && playerPos.y + viewSize.y/2 <= levelSize.y)
		view.setCenter(sf::Vector2f(viewPos.x, playerPos.y));
	else if(playerPos.y - viewSize.y/2 < 0)
		view.setCenter(sf::Vector2f(viewPos.x, viewSize.y/2));
	else if(playerPos.y + viewSize.y/2 > levelSize.x)
		view.setCenter(sf::Vector2f(viewPos.x, levelSize.y - viewSize.y/2));

	window.setView(view);

	//Update the light circle to follow the player
	//Note: Since the circle's being drawn to "view coordinates" instead of "global coordinates",
	// we've got to correct the player's position if we want the circle to be drawn on top of
	// the player
	//Theory behind this bit is that view coordinates are just global coordinates translated;
	// the amount of the translation is the distance from the global axes to the view axes
	sf::Vector2f viewcorrection(viewPos.x - viewSize.x/2, viewPos.y - viewSize.y/2);
	//Also, for some reason view coordinates have a reversed y-axis... ?_?
	circle.setPosition(sf::Vector2f(playerPos.x - viewcorrection.x, viewSize.y-(playerPos.y - viewcorrection.y)));

	//printf("playerpos: (%g, %g), correction:(%g, %g)\n", playerPos.x, playerPos.y, correctpos.x, correctpos.y);

	//Update the overlay rectangle to follow the view
	//rect.setPosition(view.getCenter());
	overlaySprite.setPosition(view.getCenter());

	overlayTexture.clear(sf::Color(0,0,0,0));
	overlayTexture.draw(rect);
	overlayTexture.draw(circle, sf::RenderStates(sf::BlendMode::BlendNone));
}

void Game::Render()
{
	window.clear();

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
	window.draw(overlaySprite);

	menus.Display(window);

	//Update the window after the drawing
	window.display();
}

void Game::Exit()
{
	isRunning = false;
}