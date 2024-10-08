#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

#include "scenes/intro/MainMenu.hpp"
#include "scenes/mission/Mission.hpp"
#include "effects/ScreenBlackoutEffect.hpp"
#include "game/Game.hpp"

int main()
{
    Game game;

    AssetManager assets;

    ScreenBlackoutEffect fade_effect;

    auto& window = game.window;
    auto& viewport = game.viewport;
    auto& visible_area = game.visible_area;
    auto& clock = game.clock;

    visible_area = sf::FloatRect(0.f, 0.f, 800.0f, 600.0f);

    uint32_t width  = static_cast<uint32_t>(visible_area.width);
    uint32_t height = static_cast<uint32_t>(visible_area.height);

    window.create(sf::VideoMode(width, height), "Dune: The Battle For Arrakis");
    window.setVerticalSyncEnabled(true);

    auto main_menu = std::make_unique<MainMenu>(game);
    auto mission =  std::make_unique<Mission>(game); 

    if(!main_menu->load(std::string()))
        return -1;

    Scene* current_scene = main_menu.get();

    viewport.reset(visible_area);
    
    while (window.isOpen())
    {
        sf::Event event;

        auto dt = clock.restart();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Resized)
            {
                const auto& center = viewport.getCenter();

                float x = center.x - event.size.width * 0.5f;
                float y = center.y - event.size.height * 0.5f;
                visible_area = sf::FloatRect(x, y, event.size.width, event.size.height);
                viewport.reset(visible_area);
            }

            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();

                return 0;
            }
        }

        current_scene->update(dt);

        if(game.sceneNeedToBeChanged)
        {
            fade_effect.prepare(viewport.getCenter(), window.getSize());
            
            while(!fade_effect.isOver())
            {     
                fade_effect.update();

                window.clear();
                window.draw(*current_scene);
                window.draw(fade_effect);
                window.display();
            }
            
            switch (game.next_scene)
            {
                case Game::GameScene::MAIN_MENU:
                {
                    current_scene = main_menu.get();
                    const auto& size = viewport.getSize();
                    visible_area = { 0, 0, size.x, size.y };
                    viewport.reset(visible_area);
                    break;
                }

                case Game::GameScene::MISSION:
                {
                    mission->load("Atreides-8.tmx");
                    current_scene = mission.get();
                    break;
                }

                default:
                    break;
            }

            game.sceneNeedToBeChanged = false;
            game.next_scene = Game::GameScene::NONE;
        }

        window.setView(viewport);
        window.clear();
        window.draw(*current_scene);
        window.display();
    }
    
    return 0;
}