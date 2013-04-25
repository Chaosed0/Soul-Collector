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

	/**
	 * Sets the active menu to either the win or the lose menu.
	 * \param win If true, sets the menu to the win menu; otherwise, sets the menu
	 *		to the lose menu.
	 */
	void WinLose(bool win);

	/**
	 * Toggles display of a loading screen.
	 */
	void ToggleLoading();
private:
	/**
	 * Sets the active menu to a certain menu.
	 *
	 * \param window The menu to set the current menu to.
	 */
	void SetActiveMenu(sfg::Window::Ptr& window);

	// --- Button callbacks ---
	void gotoHelp();
	void gotoMain();
	void gotoSettings();
	void playGame();

	/** Flag set to true when the active menu is visible */
	bool visible;

	std::vector<sfg::Window::Ptr> menus;

	sfg::Window::Ptr curWindow;

	// --- Main Menu
	sfg::Box::Ptr mainMenu;
	sfg::Table::Ptr mainLayout;
	sfg::Label::Ptr mainLabel;
	sfg::Button::Ptr playButton;
	sfg::Button::Ptr helpButton;
	sfg::Button::Ptr exitButton;
	sfg::Container::Ptr menuPtr;

	// --- No Selection
	//sfg::Box::Ptr emptyBox;
	sfg::Table::Ptr noSelection;

	// --- Help
	sfg::Table::Ptr helpMenu;
	sfg::Label::Ptr helpLabel;
	sfg::Label::Ptr controlsLabel;
	sfg::Label::Ptr textBody;
	sfg::Button::Ptr upKey;
	sfg::Button::Ptr downKey;
	sfg::Button::Ptr leftKey;
	sfg::Button::Ptr rightKey;
	sfg::Button::Ptr attackKey;
	sfg::Button::Ptr lightKey;
	sfg::Button::Ptr actKey;
	sfg::Button::Ptr runKey;
	sfg::Label::Ptr moveLabel;

	// --- Game over menu
	sfg::Box::Ptr gameOverLayout;
	sfg::Label::Ptr gameOverLabel;
	sfg::Button::Ptr gameOverBackButton;
	sfg::Button::Ptr gameOverExitButton;

	// --- Win menu
	sfg::Box::Ptr winLayout;
	sfg::Label::Ptr winLabel;
	sfg::Button::Ptr winBackButton;
	sfg::Button::Ptr winExitButton;

	// -- Settings menu
	sfg::Box::Ptr settingsMenu;
	sfg::Button::Ptr settingsButton;
	sfg::Label::Ptr settingsLabel;

	sfg::SFGUI gui;
	sfg::Desktop desktop;

	bool loading;
	sfg::Window::Ptr beforeLoading;
};

//Note: Templates must be defined in the same file they are defined
template <class Class>
void MenuManager::connectExitFunc(void (Class::*function)(), Class* object)
{
	exitButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(function, object);
	gameOverExitButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(function, object);
	winExitButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(function, object);
}

#endif