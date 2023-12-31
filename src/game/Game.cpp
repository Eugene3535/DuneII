#include "utils/Defines.hpp"

#include "scenes/intro/MainMenu.hpp"
#include "scenes/battle/BattleField.hpp"

#include "game/Game.hpp"

Game::Game(SceneNode* root) noexcept:
    SceneNode(nullptr)
{
    m_visibleArea = sf::FloatRect(0.f, 0.f, 800.0f, 600.0f);

    unsigned width  = static_cast<unsigned>(m_visibleArea.width);
    unsigned height = static_cast<unsigned>(m_visibleArea.height);

    m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "Dune: The Battle For Arrakis");
    m_window->setFramerateLimit(60U);
    m_viewport.reset(m_visibleArea);

    if( m_isLoaded = setScene<MainMenu>(); ! isLoaded() )
        return;

    switchScene();

    m_clock.restart();
}

Game::~Game()
{
}

void Game::update(float dt) noexcept
{
    sf::Event event;

    while (m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Resized)
        {
            const auto& center = m_viewport.getCenter();
            const auto& size = m_viewport.getSize();
            m_visibleArea = sf::FloatRect(center.x - size.x * 0.5f, center.y - size.y * 0.5f, event.size.width, event.size.height);
            m_viewport.reset(m_visibleArea);
        }

        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            m_window->close();
            disable();

            return;
        }
    }

    m_currentScene->update(dt);

    if(m_currentScene->isDone())
        switchScene();

    m_window->clear();
    m_window->draw(*this);
    m_window->display();
}

int Game::run() noexcept
{
    if(m_isDone)
        return 0;

    while (m_window->isOpen())
    {
        update(m_clock.restart().asSeconds());
    }

    return 0;
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