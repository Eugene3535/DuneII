#include "utils/Defines.hpp"
#include "utils/Resources.hpp"

#include "scenes/intro/MainMenu.hpp"
#include "scenes/battle/BattleField.hpp"

#include "game/Game.hpp"

Game::Game(SceneNode* root) noexcept:
    SceneNode(nullptr)
{
}

Game::~Game()
{
}

bool Game::load() noexcept
{
    m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Dune: The Battle For Arrakis");
    m_window->setFramerateLimit(60U);

    m_currentScene = pushScene<MainMenu>();

    if(!m_currentScene->load())
        return false;
    
    return true;
}

void Game::update(float dt) noexcept
{
    if(m_isDone)
        return;

    while (m_window->isOpen())
    {
        sf::Event event;

        while (m_window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                m_window->close();
                disable();

                return;
            }
        }

        m_currentScene->update(0);

        if(m_currentScene->isDone())
        {
            if(m_nextScene)
            {
                m_currentScene = m_nextScene;
                m_currentScene->enable();
                m_currentScene->update(0);
                m_nextScene = nullptr;
            }
        }

        m_window->clear();
        m_window->draw(*this);
        m_window->display();
    }
}

sf::Vector2i Game::getCursorPosition() const noexcept
{
    return sf::Mouse::getPosition(*m_window);
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!m_isDone)
        m_window->draw(*m_currentScene);
}