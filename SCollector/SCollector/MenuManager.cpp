
#include "MenuManager.h"

MenuManager::MenuManager(int winWidth, int winHeight)
{
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));
	menus.push_back(sfg::Window::Create(sfg::Window::BACKGROUND));

	win0Box = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	button1 = sfg::Button::Create("Go to menu 2");
	button1->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::button1Click, this);
	exitButton = sfg::Button::Create("Exit game");
	win0Box->Pack(button1);
	win0Box->Pack(exitButton);
	menus[0]->Add(win0Box);

	win1Box = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	button2 = sfg::Button::Create("Go to menu 3");
	button2->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::button2Click, this);
	playButton = sfg::Button::Create("Play game");
	playButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::playGame, this);
	win1Box->Pack(button2);
	win1Box->Pack(playButton);
	menus[1]->Add(win1Box);

	win2Box = sfg::Box::Create(sfg::Box::VERTICAL, 10.0f);
	button3 = sfg::Button::Create("Go to menu 1");
	button3->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::button3Click, this);
	playSoundButton = sfg::Button::Create("Sound Test");
	playSoundButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(&MenuManager::playSound, this);
	win2Box->Pack(button3);
	win2Box->Pack(playSoundButton);
	menus[2]->Add(win2Box);
	
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
	SetActiveMenu(menus[2]);
}

void MenuManager::button3Click()
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