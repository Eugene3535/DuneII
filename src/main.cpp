#include <thread>
#include <random>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "scenes/intro/TitleScreen.hpp"
#include "scenes/choosing_houses/Destiny.hpp"
#include "scenes/mission/Mission.hpp"
#include "assets/AssetManager.hpp"
#include "effects/blackout/ScreenBlackoutEffect.hpp"
#include "effects/particles/ParticleSystem.hpp"
#include "game/DuneII.hpp"


#define DEFAULT_SCREEN_WIDTH 1200
#define DEFAULT_SCREEN_HEIGHT 900


int main()
{
    std::random_device rd;
    std::mt19937       rng(rd());

    DuneII game;

    ScreenBlackoutEffect fade_effect;
    ParticleSystem ps(1000, rng);
    ps.setEmitter({ DEFAULT_SCREEN_WIDTH, (DEFAULT_SCREEN_HEIGHT >> 1) });
    ps.setRespawnArea({0, DEFAULT_SCREEN_HEIGHT});
    ps.setDirection(180);
    ps.setMaxLifetime(sf::seconds(25));
    ps.setDistribution({{ 0, 0 }, { DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT }});
    ps.setFading(true);

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
        ps.update(dt);

        if(auto [nextScene, needToBeChanged] = current_scene->getStatus(); needToBeChanged)
        {
            if(game.checkSceneRights(current_scene, nextScene))
            {
                current_scene->reset();

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
                    case Scene::Type::MAIN_MENU:
                    {
                        current_scene = titleScreen;
                    }
                    break;

                    case Scene::Type::CHOOSE_DESTINY:
                    {
                        if(auto destiny = game.load<Destiny>({}))
                            current_scene = destiny;
                    }
                    break;

                    case Scene::Type::MISSION:
                    {
                        if(auto mission = game.load<Mission>("Atreides-8.tmx"))
                            current_scene = mission;
                    }
                    break;

                    default:
                        break;
                }

                current_scene->resize(sf::Vector2f(window.getSize())); 
            }
        }

        window.setView(current_scene->getView());
        window.clear();
        
        window.draw(*current_scene);

        if(dynamic_cast<TitleScreen*>(current_scene) == titleScreen)
            window.draw(ps);

        window.display();
    }
    
    return 0;
}