#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "game/Game.hpp"
#include "scenes/intro/MainMenu.hpp"

MainMenu::MainMenu(Game& game) noexcept:
    Scene(game)
{

}

MainMenu::~MainMenu()
{
}

bool MainMenu::load(const std::string& info) noexcept
{
    if(m_isLoaded)
        return true;

    if(auto assets = Assets; assets != nullptr)
    {
        auto font = assets->getResource<sf::Font>("AvanteNrBook.ttf");
        auto texture = assets->getResource<sf::Texture>("Dune.png");

        if(!(font && texture))
            return false;

        m_planet.setTexture(*texture);

        m_startGame.setFont(*font);
        m_settings.setFont(*font);
        m_tutorial.setFont(*font);

        const std::string textStart("Начать новую игру");
        const std::string textSettings("Настройки");
        const std::string textTutorial("Обучение");

        m_startGame.setString(sf::String::fromUtf8(textStart.begin(), textStart.end()));
        m_settings.setString(sf::String::fromUtf8(textSettings.begin(), textSettings.end()));
        m_tutorial.setString(sf::String::fromUtf8(textTutorial.begin(), textTutorial.end()));

        m_startGame.setPosition(550, 400);
        m_settings.setPosition(550, 450);
        m_tutorial.setPosition(550, 500);

        m_startGame.setScale(0.5f, 0.5f);
        m_settings.setScale(0.5f, 0.5f);
        m_tutorial.setScale(0.5f, 0.5f);

        m_isLoaded = true;

        return m_isLoaded;
    }
        
    return false;
}

void MainMenu::update(sf::Time dt) noexcept
{
    if(m_game.sceneNeedToBeChanged)
        return;

    sf::Vector2f mouse_pos { sf::Mouse::getPosition(m_game.window) };

    auto is_button_pressed = [](sf::Text& text, const sf::Vector2f& mouse_pos) noexcept
    {
        if(text.getGlobalBounds().contains(mouse_pos))
        {
            text.setFillColor(sf::Color::Red);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))       
                return true;   
        }
        else   
            text.setFillColor(sf::Color::White);
        
        return false;
    };

    if(is_button_pressed(m_startGame, mouse_pos))
    {
        m_game.sceneNeedToBeChanged = true;
        m_game.next_scene = Game::GameScene::MISSION;

        return;
    }

    if(is_button_pressed(m_settings, mouse_pos))
    {
        return;
    }

    if(is_button_pressed(m_tutorial, mouse_pos))
    {
        return;
    }
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_planet, states);
    target.draw(m_startGame, states);
    target.draw(m_settings, states);
    target.draw(m_tutorial, states);
}