
#include "MenuManager.h"

MenuManager::MenuManager(int winWidth, int winHeight)
{
	loading = false;

	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));

	mainMenu = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	mainLabel = sfg::Label::Create("Soul Collector");
	playButton = sfg::Button::Create("Start");
	playButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::playGame, this);
	helpButton = sfg::Button::Create("Help");
	helpButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::gotoHelp, this);
	exitButton = sfg::Button::Create("Exit");
	noSelection = sfg::Table::Create();
	noSelection->Attach(sfg::Box::Create(), sf::Rect<sf::Uint32>(1,1,1,1));
	menuPtr = noSelection;

	mainLayout = sfg::Table::Create();
	mainLayout->Attach(mainLabel, sf::Rect<sf::Uint32>(1,1,10,5));
	mainLayout->Attach(playButton, sf::Rect<sf::Uint32>(1,6,3,1)); 
	mainLayout->Attach(helpButton, sf::Rect<sf::Uint32>(1,7,3,1));
	mainLayout->Attach(exitButton, sf::Rect<sf::Uint32>(1,8,3,1));
	mainLayout->Attach(menuPtr, sf::Rect<sf::Uint32>(6,6,5,4), 0, 0);
	/*sfg::Label::Ptr alabel= sfg::Label::Create("watatatat");
	mainLayout->Attach(alabel, sf::Rect<sf::Uint32>(6,6,5,4),
		sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND);*/

	menus[0]->Add(mainLayout);
	
	helpMenu = sfg::Table::Create();
	helpLabel = sfg::Label::Create("Help");
	controlsLabel = sfg::Label::Create("Controls");
	textBody = sfg::Label::Create("Find your way through the labyrinthine purgatory to find the shattered pieces of your soul. Use your lighter sparingly and get around the beasts by either attacking them head on or avoiding them.\n\n\
	You are slowly transforming into a beast. Being beast-like grants you heightened strength, but once you fully turn, you can never leave purgatory. Utilizing this extra strength speeds up the process as well.");
	textBody->SetLineWrap(true);
	upKey = sfg::Button::Create("W (Up)");
	downKey = sfg::Button::Create("S (Down)");
	leftKey = sfg::Button::Create("A (Left)");
	rightKey = sfg::Button::Create("D (Right)");
	lightKey = sfg::Button::Create("F (Lighter)");
	actKey = sfg::Button::Create("E (Use)");
	attackKey = sfg::Button::Create("Space (Attack)");
	runKey = sfg::Button::Create("Shift (Run)");
	moveLabel = sfg::Label::Create("Move");

	/*helpMenu->Attach(sfg::Box::Create(), sf::Rect<sf::Uint32>(1,1,9,13),
		sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sfg::Table::EXPAND);*/
	helpMenu->Attach(helpLabel, sf::Rect<sf::Uint32>(1,1,9,1),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(textBody, sf::Rect<sf::Uint32>(1,2,8,3),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(controlsLabel, sf::Rect<sf::Uint32>(1,8,8,1),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(upKey, sf::Rect<sf::Uint32>(3,9,1,1),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(moveLabel, sf::Rect<sf::Uint32>(4,9,1,1),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(downKey, sf::Rect<sf::Uint32>(3,10,1,1),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(leftKey, sf::Rect<sf::Uint32>(2,10,1,1),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(rightKey, sf::Rect<sf::Uint32>(4,10,1,1),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(actKey, sf::Rect<sf::Uint32>(4,9,1,1),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(lightKey, sf::Rect<sf::Uint32>(6,10,2,1),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(runKey, sf::Rect<sf::Uint32>(1,12,2,1),
		sfg::Table::FILL, sfg::Table::FILL);
	helpMenu->Attach(attackKey, sf::Rect<sf::Uint32>(4,12,2,1),
		sfg::Table::FILL, sfg::Table::FILL);

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

	// --- Loading menu ---
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));
	menus[3]->Add(sfg::Label::Create("Loading..."));

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

void MenuManager::gotoHelp()
{
	mainLayout->Remove(menuPtr);
	menuPtr = helpMenu;
	mainLayout->Attach(menuPtr, sf::Rect<sf::Uint32>(6,6,5,4), 0, 0);

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

void MenuManager::ToggleLoading()
{
	loading = !loading;
	if(loading) {
		beforeLoading = curWindow;
		SetActiveMenu(menus[3]);
	} else {
		SetActiveMenu(beforeLoading);
	}
}