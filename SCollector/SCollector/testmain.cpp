#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

class HelloWorld {
	public:
		// Our button click handler.
		void GotoTable1();
		void GotoTable2();

		void Run();

	private:
		// Create an SFGUI. This is required before doing anything with SFGUI.
		sfg::SFGUI m_sfgui;

		//Create the tables
		sfg::Table::Ptr outerTable;
		sfg::Table::Ptr innerTable1;
		sfg::Table::Ptr innerTable2;

		// Create the label pointer here to reach it from OnButtonClick().
		sfg::Table::Ptr innerTablePtr;
};

void HelloWorld::GotoTable1()
{
	outerTable->Remove(innerTablePtr);
	innerTablePtr = innerTable1;
	outerTable->Attach(innerTablePtr, sf::Rect<sf::Uint32>(2,2,2,2), 0, 0);
}

void HelloWorld::GotoTable2()
{
	outerTable->Remove(innerTablePtr);
	innerTablePtr = innerTable2;
	outerTable->Attach(innerTablePtr, sf::Rect<sf::Uint32>(2,2,2,2), 0, 0);
}

void HelloWorld::Run() {
	// Create SFML's window.
	sf::RenderWindow render_window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "Hello world!");

	//Init the tables
	outerTable = sfg::Table::Create();
	innerTable1 = sfg::Table::Create();
	innerTable2 = sfg::Table::Create();

	sfg::Table::Ptr emptyTable(sfg::Table::Create());
	sfg::Label::Ptr titleLabel(sfg::Label::Create("Table Test"));
	sfg::Button::Ptr gotoTable1(sfg::Button::Create("Goto Table 1"));
	sfg::Button::Ptr gotoTable2(sfg::Button::Create("Goto Table 2"));
	sfg::Box::Ptr emptyBox(sfg::Box::Create());
	gotoTable1->GetSignal( sfg::Widget::OnLeftClick ).Connect( &HelloWorld::GotoTable1, this );
	gotoTable2->GetSignal( sfg::Widget::OnLeftClick ).Connect( &HelloWorld::GotoTable2, this );
	outerTable->Attach(titleLabel, sf::Rect<sf::Uint32>(1,1,4,1));
	outerTable->Attach(gotoTable1, sf::Rect<sf::Uint32>(1,2,1,1));
	outerTable->Attach(gotoTable2, sf::Rect<sf::Uint32>(1,3,1,1));
	outerTable->Attach(emptyBox, sf::Rect<sf::Uint32>(2,2,2,2),
		sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND);
	innerTablePtr = emptyTable;
	outerTable->Attach(innerTablePtr, sf::Rect<sf::Uint32>(2,2,2,2), 0, 0);

	//Put some stuff inside the tables
	sfg::Label::Ptr label1(sfg::Label::Create("\
Etiam dui sapien, venenatis sed fermentum sed, tincidunt a massa. Proin mollis erat id lacus condimentum cursus ut nec quam. Aenean vulputate auctor mauris. Nulla eu arcu vitae neque sagittis egestas ac eget felis. Phasellus est lorem, cursus a fringilla eu, molestie nec lacus. Sed ut enim metus, ut sodales felis. Sed pretium, arcu nec eleifend mattis, nisi mauris mattis est, id consequat libero justo non turpis. Quisque a rhoncus neque. Donec congue, ipsum sed interdum luctus, nulla elit dapibus dui, nec ornare risus augue et est. Duis ullamcorper eleifend turpis ac faucibus. Nam accumsan volutpat leo, quis pretium turpis adipiscing ac. Nam egestas ornare erat ac venenatis. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Aliquam scelerisque lobortis arcu, at posuere nunc consectetur sed.\n\
Suspendisse potenti. Nam scelerisque tempor metus id blandit. Pellentesque eleifend sodales turpis eu euismod. Aenean consectetur vulputate est, vel imperdiet magna mattis convallis. Etiam scelerisque facilisis nulla, id bibendum sem congue in. Donec nisi nunc, dapibus quis adipiscing at, dictum at mauris. Nullam mollis bibendum sapien, ac porttitor est ultrices non. Suspendisse potenti. Nam iaculis, libero et semper interdum, turpis turpis vulputate elit, sed scelerisque dui diam non neque. Cras consectetur dapibus tincidunt. Nullam faucibus est sit amet purus facilisis aliquam rutrum nulla eleifend. Suspendisse suscipit imperdiet libero, tincidunt tristique nunc bibendum non. Phasellus vel ligula dolor, vitae imperdiet justo."));
	label1->SetLineWrap(true);
	sfg::Button::Ptr button1(sfg::Button::Create("button 1"));
	innerTable1->Attach(label1, sf::Rect<sf::Uint32>(1,1,1,1),
		sfg::Table::FILL, sfg::Table::FILL);
	innerTable1->Attach(button1, sf::Rect<sf::Uint32>(1,2,1,1),
		sfg::Table::FILL, sfg::Table::FILL);

	sfg::Label::Ptr label2(sfg::Label::Create("\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean adipiscing libero quis magna scelerisque gravida. Sed consequat nibh non metus consequat rutrum. Aliquam euismod quam non enim vestibulum et iaculis enim adipiscing. Nam mi nulla, faucibus id laoreet sed, eleifend in neque. Quisque condimentum rhoncus tempus. Ut lacus dolor, molestie nec pretium eu, scelerisque pulvinar est. Phasellus mollis accumsan nunc, eget vestibulum erat sagittis nec. Integer lobortis leo at dui placerat vehicula. Nulla facilisi. Sed ut dignissim risus. Donec euismod lacus et ipsum luctus non pellentesque leo fringilla. Suspendisse vitae dui lectus, vitae porta felis.\n\
Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Nam odio eros, pulvinar eget dapibus consectetur, vehicula in massa. Proin pharetra egestas nisl, quis commodo enim vulputate eget. Proin nisi dui, mattis et tempus tristique, faucibus a enim. Phasellus ultrices nunc sit amet nunc elementum vitae porttitor eros semper. Morbi et ipsum quis diam iaculis iaculis quis at nibh. Vestibulum auctor, purus vitae bibendum consectetur, eros turpis vestibulum risus, vitae tincidunt erat leo ut odio. Curabitur malesuada tellus a felis varius a consequat enim fringilla. Cras ultrices hendrerit est quis mattis. Proin ultricies fermentum magna, vel malesuada est consectetur vitae. In dapibus, est at faucibus porta, odio neque euismod ligula, ac pharetra metus velit ac nisi. Duis ac justo vitae lacus rhoncus ullamcorper. Suspendisse feugiat consectetur nisi sit amet blandit. Nulla tincidunt venenatis ligula quis adipiscing.\n"));
	label2->SetLineWrap(true);
	sfg::Button::Ptr button2(sfg::Button::Create("button 2"));
	innerTable2->Attach(label2, sf::Rect<sf::Uint32>(1,1,1,1),
		sfg::Table::FILL, sfg::Table::FILL);
	innerTable2->Attach(button2, sf::Rect<sf::Uint32>(1,2,1,1), 
		sfg::Table::FILL, sfg::Table::FILL);

	// Create a window and add the table to it
	sfg::Window::Ptr window(sfg::Window::Create(sfg::Window::BACKGROUND));
	window->SetRequisition(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	window->Add(outerTable);

	// Create a desktop and add the window to it.
	sfg::Desktop desktop;
	desktop.Add( window );

	// We're not using SFML to render anything in this program, so reset OpenGL
	// states. Otherwise we wouldn't see anything.
	render_window.resetGLStates();

	// Main loop!
	sf::Event event;
	sf::Clock clock;

	while( render_window.isOpen() ) {
		// Event processing.
		while( render_window.pollEvent( event ) ) {
			desktop.HandleEvent( event );

			// If window is about to be closed, leave program.
			if( event.type == sf::Event::Closed ) {
				render_window.close();
			}
		}

		// Update SFGUI with elapsed seconds since last call.
		desktop.Update( clock.restart().asSeconds() );

		// Rendering.
		render_window.clear();
		m_sfgui.Display( render_window );
		render_window.display();
	}
}

int main() {
	HelloWorld hello_world;
	hello_world.Run();

	return 0;
}
