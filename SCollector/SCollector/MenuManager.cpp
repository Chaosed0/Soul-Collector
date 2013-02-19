
#include "MenuManager.h"

MenuManager::MenuManager(int winWidth, int winHeight)
{
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));

	mainMenu = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	playButton = sfg::Button::Create("Start");
	playButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::playGame, this);
	helpButton = sfg::Button::Create("Help");
	helpButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::gotoHelp, this);
	settingsButton = sfg::Button::Create("Settings");
	settingsButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::gotoSettings, this);
	exitButton = sfg::Button::Create("Exit");
	mainMenu->Pack(playButton, false, false);
	mainMenu->Pack(helpButton, false, false);
	mainMenu->Pack(settingsButton, false, false);
	mainMenu->Pack(exitButton, false, false);
	menus[0]->Add(mainMenu);

	settingsMenu = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	mainMenuButton = sfg::Button::Create("Return");
	mainMenuButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::gotoMain, this);
	settingsMenu->Pack(mainMenuButton, false, false);
	menus[1]->Add(settingsMenu);
	
	helpMenu = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	helpMenu->Pack(mainMenuButton, false, false);
	menus[2]->Add(settingsMenu);

	for(int i = 0; i < menus.size(); i++)
		menus[i]->SetRequisition(sf::Vector2f(winWidth, winHeight));

	for(int i = 0; i < menus.size(); i++)
		desktop.Add(menus[i]);

	curWindow = menus[0];
	for(int i = 1; i < menus.size(); i++)
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
	SetActiveMenu(menus[1]);
}

void MenuManager::gotoHelp()
{
	SetActiveMenu(menus[2]);
}

void MenuManager::gotoMain()
{
	SetActiveMenu(menus[0]);
}

void MenuManager::playGame()
{
	SetVisible(false);
}

void MenuManager::playSound()
{

}