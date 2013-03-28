/*
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>

sfg::Table::Ptr outerbox;
sfg::Box::Ptr innerbox1;
sfg::Box::Ptr innerbox2;
sfg::Box::Ptr nothing;
sfg::Button::Ptr hide1;
sfg::Button::Ptr hide2;
sfg::Button::Ptr show1;
sfg::Button::Ptr show2;
sfg::Box::Ptr selection;

void showbox1()
{
	innerbox1->Show(true);
}

void showbox2()
{
	innerbox2->Show(true);
}

void hidebox1()
{
	innerbox1->Show(false);
}

void hidebox2()
{
	innerbox2->Show(false);
}

int main(int argc, char* argv[])
{
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "lel");
	sfg::SFGUI sfgui;
	sfg::Desktop desktop;

	sfg::Window::Ptr sfgWindow = sfg::Window::Create(sfg::Window::BACKGROUND);
	sfgWindow->SetRequisition(sf::Vector2f(800, 600));
	sfgWindow->SetPosition(sf::Vector2f(0, 0));

	outerbox = sfg::Table::Create();
	innerbox1 = sfg::Box::Create();
	innerbox2 = sfg::Box::Create();
	nothing = sfg::Box::Create();
	hide1 = sfg::Button::Create("hide box 1");
	hide2 = sfg::Button::Create("hide box 2");
	show1 = sfg::Button::Create("show box 1");
	show2 = sfg::Button::Create("show box 2");
	selection = nothing;

	desktop.Add(sfgWindow);
	innerbox1->Pack(hide1);
	innerbox2->Pack(hide2);

	sfgWindow->Add(outerbox);
	outerbox->Attach(show1, sf::Rect<sf::Uint32>(0, 0, 1, 1));
	outerbox->Attach(show2, sf::Rect<sf::Uint32>(0, 1, 1, 1));
	outerbox->Attach(innerbox1, sf::Rect<sf::Uint32>(1, 0, 1, 2));
	outerbox->Attach(innerbox2, sf::Rect<sf::Uint32>(1, 0, 1, 2));
	innerbox1->Show(false);
	innerbox2->Show(false);

	show1->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(showbox1);
	hide1->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(hidebox1);
	show2->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(showbox2);
	hide2->GetSignal(sfg::Button::OnMouseLeftRelease).Connect(hidebox2);

	bool running = true;

	while(running) {

		sf::Event event;
		while(window.pollEvent(event)) {
			switch(event.type)
			{
			case sf::Event::Closed:
				running = false;
			}
			desktop.HandleEvent(event);
		}

		window.clear();

		desktop.Update(1.0f);
		sfgui.Display(window);

		window.display();
	}
}*/
