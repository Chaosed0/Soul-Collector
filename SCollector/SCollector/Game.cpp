
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "MenuManager.h"

const sf::Time Game::fadeoutTime = sf::seconds(3.0f);
const std::string Game::firstLevel = "Debug1.tmx";
const std::string Game::firstSpawn = "Init";

Game::Game()
	: winWidth(800), winHeight(600)
	, window(sf::VideoMode(winWidth, winHeight), "Test")
	, menus(winWidth, winHeight)
	, view(sf::FloatRect(0.0f, 0.0f, (float)winWidth, (float)winHeight))
	, hud(sf::Vector2f(0, (float)winHeight))
	, fadeoutRect(sf::Vector2f((float)winWidth, (float)winHeight))
{
	//Flag set to false when the game ends
	isRunning = false;
	paused = true;

	window.setView(view);

	hud.AddObject("assets/img/hud_heart.png");
	hud.AddObject("assets/img/hud_lighter.png");
	hud.AddObject("assets/img/hud_soul_beast000.png");
	hud.AddObject("assets/img/hud_soul_beast000.png");
	
	soundManager.AddSound("assets/sound/init.ogg", "init", false);

	fadeoutRect.setFillColor(sf::Color(0,0,0,0));
	fadeoutRect.setPosition(0,0);
	fadeoutTimer = sf::Time::Zero;
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

	soundManager.PlaySound("init");
}

void Game::Loop()
{
	//Timer for regulating framerate - for an explanation of this, go to the end of
	// the main game loop
	sf::Clock frameTimer;
	//How many frames per second we want the game to run
	const int ticksPerSec = 60;
	//The time needed for that frame
	sf::Time frameTime = sf::microseconds(sf::seconds(1).asMicroseconds()/ticksPerSec);

	updateTimer.restart();

	// --- MAIN GAME LOOP ---
	while(isRunning)
	{
		//Reset the framerate regulation timer
		frameTimer.restart();

		Event();
		Update();
		Render();

		// --- FRAME CORRECTION ---
		//The theory behind this bit is that with faster and faster hardware, the functions and
		// such in this for loop will run faster and faster. We don't want that to happen, since
		// the movement code is here - the player speed shouldn't depend on the hardware! So, we
		// force the game to wait for a bit if we processed the loop too fast.
		sf::Time timeSleep = frameTime - frameTimer.getElapsedTime();
		if(timeSleep.asMicroseconds() >= 0)
			sf::sleep(timeSleep);
		//printf("Frame length: %lld milliseconds\n", frameTimer.getElapsedTime());
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
		//This event needs to go before the menu receives the input, so that 
		// the window can still close if the menu's open
		if(anEvent.type == sf::Event::Closed) {
			Exit();
		}
		//Pass the menu events and don't pass the game events if the menu is visible
		else if(menus.IsVisible()) {
			paused = true;
			menus.HandleEvent(anEvent);
			//If the menu just turned invisible after handling an event, restart the
			// update timer so that entities don't think they were simulating while
			// the menu was up
			if(!menus.IsVisible()) {
				paused = false;
			}
		}
		
		//If no level was loaded before and we are now in the game, load a level
		if(!paused && !levelManager.HasLoadedLevel()) {
			levelManager.LoadMap(firstLevel, firstSpawn);
		}

		//If the game is not paused, allow it to receive events
		if(!paused) {
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
				case sf::Keyboard::W :
					levelManager.GetPlayer().MoveUp(true);
					break;
				case sf::Keyboard::S :
					levelManager.GetPlayer().MoveDown(true);
					break;
				case sf::Keyboard::A :
					levelManager.GetPlayer().MoveLeft(true);
					break;
				case sf::Keyboard::D :
					levelManager.GetPlayer().MoveRight(true);
					break;
				case sf::Keyboard::F :
					levelManager.GetPlayer().ToggleLighter();
					break;
				case sf::Keyboard::E :
					levelManager.GetCurrentLevel().DoActivate();
					break;
				case sf::Keyboard::Space :
					levelManager.GetCurrentLevel().AddAttack(
						levelManager.GetCurrentLevel().GetPlayer().GetAttackCone(), false);
					break;
				case sf::Keyboard::LShift :
				case sf::Keyboard::RShift :
					levelManager.GetCurrentLevel().GetPlayer().Sprint(true);
					break;
				case sf::Keyboard::Escape :
					menus.SetVisible(true);
					break;
				default:
					break;
				}
				break;
			case sf::Event::KeyReleased :
				switch(anEvent.key.code) {
				case sf::Keyboard::W :
					levelManager.GetPlayer().MoveUp(false);
					break;
				case sf::Keyboard::S :
					levelManager.GetPlayer().MoveDown(false);
					break;
				case sf::Keyboard::A :
					levelManager.GetPlayer().MoveLeft(false);
					break;
				case sf::Keyboard::D :
					levelManager.GetPlayer().MoveRight(false);
					break;
				case sf::Keyboard::LShift :
				case sf::Keyboard::RShift :
					levelManager.GetPlayer().Sprint(false);
					break;
				default:
					break;
				}
				break;
			default :
				break;
			}
		}
	}
}

void Game::Update()
{
	//We have to store the update time and re-get it BEFORE passing it to the level
	// because the update function is the most time consuming; the time taken within
	// the function itself is not negligible, so we can't put the restart() after it
	//Also, we need to put this outside the "if(!paused)" block so that after the game
	// isn't paused, entities don't think that time passed within the pause
	//printf("Update length: %lld milliseconds\n", updateTimer.getElapsedTime());
	sf::Time updateTime = updateTimer.getElapsedTime();
	updateTimer.restart();

	//Check win/lose conditions
	if(levelManager.HasLoadedLevel()) {
		bool win = levelManager.GetCurrentLevel().GetPlayer().GetRemainingSouls() == 0;
		bool lose = !levelManager.GetCurrentLevel().GetPlayer().IsAlive();

		if(win || lose) {
			fadeoutTimer += updateTime;
			if(win) {
				fadeoutRect.setFillColor(sf::Color(255,255,255,
					255*fadeoutTimer.asMicroseconds()/fadeoutTime.asMicroseconds()));
			} else {
				fadeoutRect.setFillColor(sf::Color(0,0,0,
					255*fadeoutTimer.asMicroseconds()/fadeoutTime.asMicroseconds()));
			}
			if(fadeoutTimer >= fadeoutTime) {
				fadeoutTimer = sf::Time::Zero;
				levelManager.Reset();
				menus.WinLose(win);
				menus.SetVisible(true);
			}
			paused = true;
		}
	}

	if(!paused) {
		sf::Vector2f playerPos = levelManager.GetCurrentLevel().GetPlayer().GetPos();
		sf::Vector2i levelSize = levelManager.GetCurrentLevel().GetSize();
		sf::Vector2f viewSize = view.getSize();
		sf::Vector2f viewPos = view.getCenter();

		//Update the level
		levelManager.Update(updateTime);

		//Update the HUD with values from the level
		levelManager.GetCurrentLevel().UpdateHUD(hud);

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
	}
}

void Game::Render()
{
	// --- RENDERING ---
	//Clear the screen of what was drawn before
	// This is inefficient, but if we don't call it, then there will be artefacts from the
	// previous frame
	window.clear();

	//Render the level to the screen
	if(levelManager.HasLoadedLevel()) {
		window.draw(levelManager.GetCurrentLevel());
	}

	//Render non-scrolling objects in the default view
	window.setView(window.getDefaultView());
	hud.draw(window);
	window.draw(fadeoutRect);
	window.setView(view);

	menus.Display(window);

	//Update the window after the drawing
	window.display();
}

void Game::Exit()
{
	isRunning = false;
}
