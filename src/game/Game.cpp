#include "managers/AssetManager.hpp"
#include "managers/SceneManager.hpp"

#include "scenes/intro/MainMenu.hpp"
#include "scenes/battle/BattleField.hpp"

#include "states/GameState.hpp"
#include "game/Game.hpp"

Game::Game() noexcept
{
}

Game::~Game()
{
}

int Game::run() noexcept
{
    sf::FloatRect m_visibleArea = sf::FloatRect(0.f, 0.f, 800.0f, 600.0f);

    unsigned width  = static_cast<unsigned>(m_visibleArea.width);
    unsigned height = static_cast<unsigned>(m_visibleArea.height);

    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "Dune: The Battle For Arrakis");
    window->setFramerateLimit(60U);

    AssetManager assets;
    SceneManager scenes;

    Scene* currentScene = nullptr;
    auto mainMenu = scenes.push<MainMenu>();
    auto battle = scenes.push<BattleField>(); // TODE: rename to Mission

    if(!mainMenu->load(std::string()))
        return -1;

    currentScene = mainMenu;
    GameState::A_SCENE_NEEDS_TO_BE_CHANGED = false;

    mainMenu->getViewport().reset(m_visibleArea);

    sf::Clock clock;

    while (window->isOpen())
    {
        sf::Event event;

        auto dt = clock.restart();
        auto& viewport = currentScene->getViewport();

        while (window->pollEvent(event))
        {

            if (event.type == sf::Event::Resized)
            {
                const auto& center = viewport.getCenter();
                const auto& size = viewport.getSize();
                m_visibleArea = sf::FloatRect(center.x - size.x * 0.5f, center.y - size.y * 0.5f, event.size.width, event.size.height);
                viewport.reset(m_visibleArea);
            }

            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window->close();

                return 0;
            }
        }

        auto mouse_pos = sf::Mouse::getPosition(*window);
        currentScene->setCursorPosition(mouse_pos);
        currentScene->update(dt);

        if(GameState::A_SCENE_NEEDS_TO_BE_CHANGED)
        {
            battle->load("Atreides-8.tmx");
            currentScene = battle;

            GameState::A_SCENE_NEEDS_TO_BE_CHANGED = false;
            GameState::next_scene = GameState::NONE;
        }

        window->setView(viewport);

        window->clear();
        window->draw(*currentScene);
        window->display();
    }
    
    return 0;
}