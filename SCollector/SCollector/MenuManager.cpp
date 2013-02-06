
#include "MenuManager.h"

MenuManager::MenuManager(int winWidth, int winHeight)
{
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));

	win0Box = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	button1 = sfg::Button::Create("Go to menu 1");
	button1->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::button1Click, this);
	exitButton = sfg::Button::Create("Exit game");
	//exitButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&
	win0Box->Pack(button1);
	win0Box->Pack(exitButton);
	menus[0]->Add(win0Box);

	win1Box = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	button2 = sfg::Button::Create("Go to menu 2");
	button2->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::button2Click, this);
	playButton = sfg::Button::Create("Play game");
	playButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::PlayGame, this);
	win1Box->Pack(button2);
	win1Box->Pack(playButton);
	menus[1]->Add(win1Box);
	
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

void MenuManager::button1Click()
{
	SetActiveMenu(menus[1]);
}

void MenuManager::button2Click()
{
	SetActiveMenu(menus[0]);
}

void MenuManager::PlayGame()
{
	SetVisible(false);
}