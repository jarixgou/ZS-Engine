#include "Common.h"
#include "Engine/ZS.h"

// Prototypes
void Init();
void Update(sf::RenderWindow& _renderWindow, float _dt);
void Display(sf::RenderWindow&);

int main()
{
	srand(time(NULL));

	sf::RenderWindow renderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Template SFML");

	ImGui::SFML::Init(renderWindow);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	Init();

	sf::Clock deltaTime;
	while (renderWindow.isOpen())
	{
		sf::Time time = deltaTime.restart();
		ImGui::SFML::Update(renderWindow, time);
		float dt = time.asSeconds();

		sf::Event event;

		while (renderWindow.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				renderWindow.close();
			}
		}

		Update(renderWindow, dt);
		Display(renderWindow);
	}
}



///////////////////////////////////////////////////////////////////
void Init()
{
	//mapEditor.Load();
	ZS::Load();
}

void Update(sf::RenderWindow& _renderWindow, float _dt)
{
	//mapEditor.Update(_dt, _renderWindow);
	ZS::Update(_renderWindow, _dt);
}

void Display(sf::RenderWindow& _window)
{
	_window.clear(sf::Color::Black);

	//mapEditor.Draw(_window);
	ZS::Draw(_window);

	ImGui::SFML::Render(_window);

	_window.display();
}