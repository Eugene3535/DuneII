#include "utils/Defines.hpp"
#include "utils/Resources.hpp"

#include "scenes/TitleScreen.hpp"
#include "scenes/BattleField.hpp"

#include "game/Game.hpp"

Game::Game(SceneNode* root) noexcept:
    SceneNode(nullptr),
    m_state(Game::State::TITLE_SCREEN)
{
}

Game::~Game()
{
}

bool Game::load() noexcept
{
    m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Dune: The Battle For Arrakis");
    m_window->setFramerateLimit(60U);

    m_currentScene = pushScene<TitleScreen>();

    if(!m_currentScene->load())
    {
        CHECK_EXPR(false);

        return false;
    }

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
                m_isDone = true;

                return;
            }
        }

        m_currentScene->update(0);

        if(m_currentScene->isDone())
        {
            if(auto* nextScene = m_scenes.back().get())
            {
                if(nextScene->load())
                {
                    m_currentScene = nextScene;
                    m_currentScene->update(0);
                }
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