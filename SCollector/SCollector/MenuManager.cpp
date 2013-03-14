
#include "MenuManager.h"

MenuManager::MenuManager(int winWidth, int winHeight)
{
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));
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
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));

	menus[0]->Add(mainLayout);

	settingsMenu = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	settingsLabel = sfg::Label::Create("Settings");
	settingstoMain = sfg::Button::Create("Return");
	settingstoMain->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::gotoMain, this);
	settingsMenu->Pack(settingsLabel, false, false);
	settingsMenu->Pack(settingstoMain);
	menus[1]->Add(settingsMenu);
	
	helpMenu = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	helpLabel = sfg::Label::Create("Help");
	helptoMain = sfg::Button::Create("Return");
	helptoMain->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::gotoMain, this);
	helpMenu->Pack(helpLabel, false, false);
	helpMenu->Pack(helptoMain);
	menus[2]->Add(helpMenu);

	for(unsigned int i = 0; i < menus.size(); i++)
		menus[i]->SetRequisition(sf::Vector2f((float)winWidth, (float)winHeight));

	for(unsigned int i = 0; i < menus.size(); i++)
		desktop.Add(menus[i]);

	curWindow = menus[0];
	for(unsigned int i = 1; i < menus.size(); i++)
		menus[i]->Show(false);

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
	if(visible)
	{
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
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	curWindow->Show(true);
}

void MenuManager::gotoHelp()
{
	curWindow->Show(false);
	mainLayout->Remove(selection);
	selection = helpMenu;
	mainLayout->Attach(selection,
		sf::Rect<sf::Uint32>(1,5,2,4),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	curWindow->Show(true);
}

void MenuManager::gotoMain()
{
	curWindow->Show(false);
	mainLayout->Remove(selection);
	selection = noSelection;
	mainLayout->Attach(selection,
		sf::Rect<sf::Uint32>(1,5,2,4),
		sfg::Table::FILL | sfg::Table::EXPAND,
		sfg::Table::FILL | sfg::Table::EXPAND,
		sf::Vector2f(10.f,10.f));
	curWindow->Show(true);
}

void MenuManager::playGame()
{
	SetVisible(false);
}
