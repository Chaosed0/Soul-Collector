
#include "MenuManager.h"

MenuManager::MenuManager(int winWidth, int winHeight)
{
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));

	mainMenu = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	mainLabel = sfg::Label::Create("Soul Collector");
	playButton = sfg::Button::Create("Start");
	playButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::playGame, this);
	helpButton = sfg::Button::Create("Help");
	helpButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::gotoHelp, this);
	settingsButton = sfg::Button::Create("Settings");
	settingsButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::gotoSettings, this);
	exitButton = sfg::Button::Create("Exit");
	noSelection = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	selection = noSelection;

	mainLayout = sfg::Table::Create();
	mainLayout->Attach(mainLabel, 
		sf::Rect<sf::Uint32>(0,0,3,5), 
		sfg::Table::FILL | sfg::Table::EXPAND, 
		sfg::Table::FILL | sfg::Table::EXPAND, 
		sf::Vector2f(10.f,10.f));
	mainLayout->Attach(playButton, 
		sf::Rect<sf::Uint32>(0,5,1,1), 
		sfg::Table::FILL | sfg::Table::EXPAND, 
		sfg::Table::FILL | sfg::Table::EXPAND, 
		sf::Vector2f(10.f,10.f));
	mainLayout->Attach(helpButton, 
		sf::Rect<sf::Uint32>(0,6,1,1), 
		sfg::Table::FILL | sfg::Table::EXPAND, 
		sfg::Table::FILL | sfg::Table::EXPAND, 
		sf::Vector2f(10.f,10.f));
	mainLayout->Attach(settingsButton, 
		sf::Rect<sf::Uint32>(0,7,1,1), 
		sfg::Table::FILL | sfg::Table::EXPAND, 
		sfg::Table::FILL | sfg::Table::EXPAND, 
		sf::Vector2f(10.f,10.f));
	mainLayout->Attach(exitButton, 
		sf::Rect<sf::Uint32>(0,8,1,1), 
		sfg::Table::FILL | sfg::Table::EXPAND, 
		sfg::Table::FILL | sfg::Table::EXPAND, 
		sf::Vector2f(10.f,10.f));
	mainLayout->Attach(selection,
		sf::Rect<sf::Uint32>(1,5,2,4),
		sfg::Table::FILL,
		sfg::Table::FILL,
		sf::Vector2f(10.f,10.f));

	menus[0]->Add(mainLayout);

	settingsMenu = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	settingsLabel = sfg::Label::Create("Settings");
	settingsMenu->Pack(settingsLabel, false, false);
	
	helpMenu = sfg::Table::Create();
	helpLabel = sfg::Label::Create("Help");
	goalLabel = sfg::Label::Create("Goal");
	controlsLabel = sfg::Label::Create("Controls");
	timeLabel = sfg::Label::Create("Time is Running Out");
	goalBody = sfg::Label::Create("Find your way through the labyrinthine purgatory to find the shattered pieces of your soul. Use your lighter sparingly and get around the beasts by either attacking them head on or utilizing traps and doors. ");
	goalBody->SetLineWrap(true);
	timeBody = sfg::Label::Create("You are slowly becoming a beast. Being beast-like grants you heightened strength, but once you fully turn, you can never leave purgatory. Utilizing this extra strength speeds up the process as well.");
	timeBody->SetLineWrap(true);
	upKey = sfg::Button::Create("W");
	downKey = sfg::Button::Create("S");
	leftKey = sfg::Button::Create("A");
	rightKey = sfg::Button::Create("D");
	actKey = sfg::Button::Create("Space");
	moveLabel = sfg::Label::Create("Move");
	actLabel = sfg::Label::Create("Action/Attack");

	helpMenu->Attach(helpLabel,
		sf::Rect<sf::Uint32>(0,0,5,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(goalLabel,
		sf::Rect<sf::Uint32>(0,1,1,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(controlsLabel,
		sf::Rect<sf::Uint32>(1,3,4,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(timeLabel,
		sf::Rect<sf::Uint32>(1,1,4,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(goalBody,
		sf::Rect<sf::Uint32>(0,2,1,5),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(timeBody,
		sf::Rect<sf::Uint32>(1,2,4,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(upKey,
		sf::Rect<sf::Uint32>(2,4,1,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(downKey,
		sf::Rect<sf::Uint32>(2,5,1,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(leftKey,
		sf::Rect<sf::Uint32>(1,5,1,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(rightKey,
		sf::Rect<sf::Uint32>(3,5,1,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(actKey,
		sf::Rect<sf::Uint32>(1,6,3,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(moveLabel,
		sf::Rect<sf::Uint32>(4,4,1,2),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	helpMenu->Attach(actLabel,
		sf::Rect<sf::Uint32>(4,6,1,1),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));

	// --- Game over menu ---
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));

	gameOverLabel = sfg::Label::Create("GAME OVER");
	gameOverBackButton = sfg::Button::Create("Try again");
	gameOverBackButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::gotoMain, this);
	gameOverExitButton = sfg::Button::Create("Quit");

	gameOverLayout = sfg::Box::Create(sfg::Box::VERTICAL, 20.0f);
	gameOverLayout->Pack(gameOverLabel, false, false);
	gameOverLayout->Pack(gameOverBackButton, false, false);
	gameOverLayout->Pack(gameOverExitButton, false, false);

	menus[1]->Add(gameOverLayout);

	// --- Win menu ---
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));

	winLabel = sfg::Label::Create("YOU WIN");
	winBackButton = sfg::Button::Create("Main Menu");
	winBackButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::gotoMain, this);
	winExitButton = sfg::Button::Create("Quit");

	winLayout = sfg::Box::Create(sfg::Box::VERTICAL, 20.0f);
	winLayout->Pack(winLabel, false, false);
	winLayout->Pack(winBackButton, false, false);
	winLayout->Pack(winExitButton, false, false);

	menus[2]->Add(winLayout);

	//Set menu default properties
	for(unsigned int i = 0; i < menus.size(); i++)
		menus[i]->SetRequisition(sf::Vector2f((float)winWidth, (float)winHeight));

	for(unsigned int i = 0; i < menus.size(); i++) {
		desktop.Add(menus[i]);
		menus[i]->Show(false);
	}

	//Set the current menu to the main menu
	menus[0]->Show(true);
	curWindow = menus[0];

	//The menu is visible by default
	visible = true;
}

bool MenuManager::IsVisible()
{
	return visible;
}

void MenuManager::SetVisible(bool vis)
{
	visible = vis;
}

void MenuManager::HandleEvent(sf::Event& anEvent)
{
	desktop.HandleEvent(anEvent);
}

void MenuManager::Display(sf::RenderTarget& target)
{
	if(visible) {
		desktop.Update(1.0f);
		gui.Display(target);
	}
}

void MenuManager::SetActiveMenu(sfg::Window::Ptr& window)
{
	curWindow->Show(false);
	curWindow = window;
	curWindow->Show(true);
}

void MenuManager::gotoSettings()
{
	curWindow->Show(false);
	mainLayout->Remove(selection);
	selection = settingsMenu;
	mainLayout->Attach(selection,
		sf::Rect<sf::Uint32>(1,5,2,4),
		sfg::Table::EXPAND,
		sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	curWindow->Show(true);

	printf("Go to Settings\n");
}

void MenuManager::gotoHelp()
{
	curWindow->Show(false);
	mainLayout->Remove(selection);
	selection = helpMenu;
	mainLayout->Attach(selection,
		sf::Rect<sf::Uint32>(1,5,2,4),
		sfg::Table::EXPAND,
		sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	curWindow->Show(true);

	printf("Go to Help\n");
}

void MenuManager::gotoMain()
{
	SetActiveMenu(menus[0]);
}

void MenuManager::playGame()
{
	SetVisible(false);
}

void MenuManager::WinLose(bool win)
{
	if(win) {
		SetActiveMenu(menus[2]);
	} else {
		SetActiveMenu(menus[1]);
	}
}