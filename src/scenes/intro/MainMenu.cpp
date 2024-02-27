#include "managers/AssetManager.hpp"
#include "states/GameState.hpp"
#include "scenes/intro/MainMenu.hpp"

MainMenu::MainMenu() noexcept
{

}

MainMenu::~MainMenu()
{
}

bool MainMenu::load(const std::string& info) noexcept
{
    if(m_isLoaded)
        return true;
        
    auto font = AssetManager::get<sf::Font>("AvanteNrBook.ttf");
    auto texture = AssetManager::get<sf::Texture>("Dune.png");

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

    return true;
}

void MainMenu::open() noexcept
{

}

void MainMenu::close() noexcept
{

}

void MainMenu::update(sf::Time dt) noexcept
{
    if(GameState::A_SCENE_NEEDS_TO_BE_CHANGED)
        return;

    sf::Vector2f point(m_cursorPosition);

    auto isButtonPressed = [](sf::Text& text, const sf::Vector2f& point) noexcept
    {
        if(text.getGlobalBounds().contains(point))
        {
            text.setColor(sf::Color::Red);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))       
                return true;   
        }
        else   
            text.setColor(sf::Color::White);
        
        return false;
    };

    if(isButtonPressed(m_startGame, point))
    {
        GameState::A_SCENE_NEEDS_TO_BE_CHANGED = true;
        GameState::next_scene = GameState::BATTLE;

        return;
    }

    if(isButtonPressed(m_settings, point))
    {
        return;
    }

    if(isButtonPressed(m_tutorial, point))
    {
        return;
    }
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_planet);
    target.draw(m_startGame);
    target.draw(m_settings);
    target.draw(m_tutorial);
}