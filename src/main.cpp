#include "game/DuneII.hpp"


int main()
{
    const uint32_t defaultScreenWidth = 1200;
    const uint32_t defaultScreenHeight = 900;

	sf::VideoMode videoMode({defaultScreenWidth, defaultScreenHeight}, 32);
	const sf::String title("Dune II: The Battle For Arrakis");

	sf::RenderWindow window(videoMode, title);
	window.setVerticalSyncEnabled(true);

	sf::Clock clock;

    DuneII game(window);

    if(!game.init(/* GameState& state */))
        return -1;

	while (window.isOpen())
	{
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }

            if (const auto* resized = event->getIf<sf::Event::Resized>())
                game.resize(resized->size);
        }

        const auto dt = clock.restart();
        game.update(dt);

        window.clear();
        window.draw(game);
        window.display();
	}

	return 0;
}
