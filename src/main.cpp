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

    visible_area = sf::FloatRect({0.f, 0.f}, {1200.f, 800.f});

    uint32_t width  = static_cast<uint32_t>(visible_area.size.x);
    uint32_t height = static_cast<uint32_t>(visible_area.size.y);

    window.create(sf::VideoMode({ width, height }), "Dune II: The Battle For Arrakis", sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    auto main_menu = std::make_unique<MainMenu>(game);
    auto mission =  std::make_unique<Mission>(game); 

    if(!main_menu->load(std::string()))
        return -1;

    Scene* current_scene = main_menu.get();

    viewport.setCenter({600.f, 400.f});
    viewport.setSize({1200.f, 800.f});
    
    while (window.isOpen())
    {
        auto dt = clock.restart();

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
            {
                const auto& center = viewport.getCenter();

                float x = center.x - resized->size.x * 0.5f;
                float y = center.y - resized->size.y * 0.5f;
                float w = static_cast<float>(resized->size.x);
                float h = static_cast<float>(resized->size.y);
                visible_area = sf::FloatRect({x, y}, {w, h});
                viewport.setCenter({x, y});
                viewport.setSize({w, h});
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