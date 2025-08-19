#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

#include "scenes/intro/MainMenu.hpp"
#include "scenes/mission/Mission.hpp"
#include "assets/AssetManager.hpp"
#include "effects/ScreenBlackoutEffect.hpp"
#include "game/DuneII.hpp"

int main()
{
    DuneII game;

    AssetManager assets;

    ScreenBlackoutEffect fade_effect;

    auto& window = game.window;
    auto& viewport = game.viewport;
    auto& visible_area = game.visible_area;
    auto& clock = game.clock;

    visible_area = sf::FloatRect({0.f, 0.f}, {1200.f, 800.f});

    uint32_t width  = static_cast<uint32_t>(visible_area.size.x);
    uint32_t height = static_cast<uint32_t>(visible_area.size.y);

    window.create(sf::VideoMode({ width, height }), "Dune II: The Battle For Arrakis", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    auto main_menu = game.load<MainMenu>({});

    if(not main_menu)
        return -1;

    Mission* mission = nullptr; 
    Scene* current_scene = main_menu;

    viewport.setCenter({600.f, 400.f});
    viewport.setSize({1200.f, 800.f});
    
    while (window.isOpen())
    {
        const auto dt = clock.restart();

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
                auto center = current_scene->resize({resized->size});
                viewport.setSize(sf::Vector2f(resized->size));
                viewport.setCenter(sf::Vector2f(center));

                float x = static_cast<float>(center.x - (resized->size.x >> 1));
                float y = static_cast<float>(center.y - (resized->size.y >> 1));
                float w = static_cast<float>(resized->size.x);
                float h = static_cast<float>(resized->size.y);
                visible_area = sf::FloatRect( { 0.f, 0.f }, { w, h } );
                viewport.setCenter({x, y});
                viewport.setSize({w, h});
            }
        }

        current_scene->update(dt);

        if(auto request = game.isSceneNeedToBeChanged(); request.second)
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

            const DuneII::GameScene requested_scene = request.first;
            
            switch (requested_scene)
            {
                case DuneII::GameScene::MAIN_MENU:
                {
                    current_scene = main_menu;
                    break;
                }

                case DuneII::GameScene::MISSION:
                {
                    if( mission = game.load<Mission>("Atreides-8.tmx"); mission)
                        current_scene = mission;

                    break;
                }

                default:
                    break;
            }

            auto center = current_scene->resize({window.getSize()});
            viewport.setCenter(sf::Vector2f(center));
            game.resetSceneChange();
        }

        window.setView(viewport);
        window.clear();
        window.draw(*current_scene);
        window.display();
    }
    
    return 0;
}