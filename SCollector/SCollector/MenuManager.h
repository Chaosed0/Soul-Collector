/**
 * \brief Manager for in-game menus.
 * \author Edward Lu
 * \date February 2 2013
 */

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
	void button1Click();
	void button2Click();
	void PlayGame();

	/** Flag set to true when the active menu is visible */
	bool visible;

	std::vector<sfg::Window::Ptr> menus;

	sfg::Window::Ptr curWindow;

	// --- Window 0 widgets
	sfg::Box::Ptr win0Box;
	sfg::Button::Ptr button1;
	sfg::Button::Ptr exitButton;

	// --- Window 1 widgets
	sfg::Box::Ptr win1Box;
	sfg::Button::Ptr button2;
	sfg::Button::Ptr playButton;

	sfg::SFGUI gui;
	sfg::Desktop desktop;
};

//Note: Templates must be defined in the same file they are defined
template <class Class>
void MenuManager::connectExitFunc(void (Class::*function)(), Class* object)
{
	exitButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(function, object);
}