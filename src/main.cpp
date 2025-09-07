#include <thread>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

#include "scenes/intro/TitleScreen.hpp"
#include "scenes/mission/Mission.hpp"
#include "assets/AssetManager.hpp"
#include "effects/blackout/ScreenBlackoutEffect.hpp"
#include "game/DuneII.hpp"


#define DEFAULT_SCREEN_WIDTH 1200
#define DEFAULT_SCREEN_HEIGHT 900


int main()
{
    DuneII game;

    ScreenBlackoutEffect fade_effect;

    auto& window = game.window;
    auto& visible_area = game.visible_area;
    auto& clock = game.clock;

    visible_area = sf::FloatRect({0.f, 0.f}, {DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT});

    uint32_t width  = static_cast<uint32_t>(visible_area.size.x);
    uint32_t height = static_cast<uint32_t>(visible_area.size.y);

    window.create(sf::VideoMode({ width, height }), "Dune II: The Battle For Arrakis", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    auto titleScreen = game.load<TitleScreen>({});

    if(!titleScreen)
        return -1;

    Mission* mission = nullptr; 
    Scene* current_scene = titleScreen;
    
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
                current_scene->resize(sf::Vector2f(resized->size));    
        }

        current_scene->update(dt);

        if(auto [nextScene, needToBeChanged] = game.isSceneNeedToBeChanged(); needToBeChanged)
        {
            fade_effect.prepare(current_scene->getView().getCenter(), window.getSize());
            
            while(!fade_effect.isOver())
            {     
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                fade_effect.update();
                window.clear();
                window.draw(*current_scene);
                window.draw(fade_effect);
                window.display();
            }
            
            switch (nextScene)
            {
                case DuneII::GameScene::MAIN_MENU:
                {
                    current_scene = titleScreen;
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

            current_scene->resize(sf::Vector2f(window.getSize())); 
            game.resetSceneChange();
        }

        window.setView(current_scene->getView());
        window.clear();
        window.draw(*current_scene);
        window.display();
    }
    
    return 0;
}