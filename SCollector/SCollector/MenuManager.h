/**
 * \brief Manager for in-game menus.
 * \author Edward Lu
 * \date February 2 2013
 */

#ifndef __MENUMANAGER_H
#define __MENUMANAGER_H

#include <SFGUI/SFGUI.hpp>

class MenuManager 
{
public:
	/**
	 * Init constructor.
	 *
	 * The menu manager needs the window width and height so it can know
	 *  what size to make its menus (windows).
	 * \param winWidth The width of the window.
	 * \param winHeight The height of the window.
	 */
	MenuManager(int winWidth, int winHeight);

	/**
	 * Passes events to SFGUI.
	 *
	 * \param anEvent An event from the SFML window.
	 */
	void HandleEvent(sf::Event& anEvent);
	/**
	 * Displays the current menu on the render target.
	 *
	 * \param The target to render to.
	 */
	void Display(sf::RenderTarget& target);

	/**
	 * Returns the visible flag of the menus.
	 *
	 * \return True if the menu is visible, false otherwise.
	 */
	bool IsVisible();
	/**
	 * Sets the menu to be visible or not.
	 *
	 * \param vis True if the menu should be visible, false otherwise.
	 */
	void SetVisible(bool vis);

	/**
	 * Registers a callback on the exit button.
	 *
	 * \param function The function to connect the exit button callback to.
	 * \param object The object that has function as a member.
	 */
	template <class Class>
	void connectExitFunc(void (Class::*function)(), Class* object);
private:
	void SetActiveMenu(sfg::Window::Ptr& window);

	// --- Button callbacks ---
	void gotoSettings();
	void gotoHelp();
	void gotoMain();
	void playGame();
	void playSound();

	/** Flag set to true when the active menu is visible */
	bool visible;

	std::vector<sfg::Window::Ptr> menus;

	sfg::Window::Ptr curWindow;

	// --- Main Menu
	sfg::Box::Ptr mainMenu;
	sfg::Button::Ptr playButton;
	sfg::Button::Ptr settingsButton;
	sfg::Button::Ptr helpButton;
	sfg::Button::Ptr exitButton;

	// --- Settings
	sfg::Box::Ptr settingsMenu;
	sfg::Button::Ptr settingstoMain;

	// --- Help
	sfg::Box::Ptr helpMenu;
	sfg::Button::Ptr helptoMain;

	sfg::SFGUI gui;
	sfg::Desktop desktop;
};

//Note: Templates must be defined in the same file they are defined
template <class Class>
void MenuManager::connectExitFunc(void (Class::*function)(), Class* object)
{
	exitButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(function, object);
}

#endif