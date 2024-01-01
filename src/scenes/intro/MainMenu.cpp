#include "managers/AssetManager.hpp"
#include "scenes/battle/BattleField.hpp"
#include "scenes/intro/MainMenu.hpp"

MainMenu::MainMenu(SceneNode* root) noexcept:
    SceneNode(root)
{
    m_state = SceneNode::State::MAIN_MENU;

    auto font = AssetManager::get<sf::Font>("AvanteNrBook.ttf");
    auto texture = AssetManager::get<sf::Texture>("Dune.png");

    if(!(font && texture))
        return;

    m_planet.setTexture(*texture);

    m_menuItems[0].setFont(*font);
    m_menuItems[1].setFont(*font);
    m_menuItems[2].setFont(*font);

    const std::string textStart("Начать новую игру");
    const std::string textOptions("Настройки");
    const std::string textTutorial("Обучение");

    m_menuItems[0].setString(sf::String::fromUtf8(textStart.begin(), textStart.end()));
    m_menuItems[1].setString(sf::String::fromUtf8(textOptions.begin(), textOptions.end()));
    m_menuItems[2].setString(sf::String::fromUtf8(textTutorial.begin(), textTutorial.end()));

    m_menuItems[0].setPosition(550, 400);
    m_menuItems[1].setPosition(550, 450);
    m_menuItems[2].setPosition(550, 500);

    m_menuItems[0].setScale(0.5f, 0.5f);
    m_menuItems[1].setScale(0.5f, 0.5f);
    m_menuItems[2].setScale(0.5f, 0.5f);

    m_isLoaded = true;
}

MainMenu::~MainMenu()
{
}

void MainMenu::update(float dt) noexcept
{
    if(m_isDone)
        return;

    sf::Vector2f point(getCursorPosition());

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

    if(isButtonPressed(m_menuItems[0], point))
    {
        m_isDone = setScene<BattleField>();

        return;
    }

    if(isButtonPressed(m_menuItems[1], point))
    {
        return;
    }

    if(isButtonPressed(m_menuItems[2], point))
    {
        return;
    }
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isDone)
        return;

    target.setView(m_viewport);     
    target.draw(m_planet);
    target.draw(m_menuItems[0]);
    target.draw(m_menuItems[1]);
    target.draw(m_menuItems[2]);
}