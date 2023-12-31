#include "managers/AssetManager.hpp"
#include "scenes/battle/BattleField.hpp"
#include "scenes/intro/MainMenu.hpp"

MainMenu::MainMenu(SceneNode* root) noexcept:
    SceneNode(root)
{
    m_state = SceneNode::State::MAIN_MENU;
}

MainMenu::~MainMenu()
{
}

bool MainMenu::load() noexcept
{
    auto font = AssetManager::get<sf::Font>("AvanteNrBook.ttf");

    if(!font)
        return false;

    m_menuItems[0].setFont(*font);
    m_menuItems[1].setFont(*font);
    m_menuItems[2].setFont(*font);

    const std::string textStart("Начать новую игру");
    const std::string textOptions("Настройки");
    const std::string textTutorial("Обучение");

    m_menuItems[0].setString(sf::String::fromUtf8(textStart.begin(), textStart.end()));
    m_menuItems[1].setString(sf::String::fromUtf8(textOptions.begin(), textOptions.end()));
    m_menuItems[2].setString(sf::String::fromUtf8(textTutorial.begin(), textTutorial.end()));

    m_menuItems[0].setPosition(250, 100);
    m_menuItems[1].setPosition(250, 250);
    m_menuItems[2].setPosition(250, 400);

    m_isLoaded = true;

    return m_isLoaded;
}

void MainMenu::update(float dt) noexcept
{
    if(m_isDone)
        return;

    sf::Vector2f point(getCursorPosition());

    auto change_color = [this](sf::Text& text, const sf::Vector2f& point)
    {
        if(text.getGlobalBounds().contains(point))
        {
            text.setColor(sf::Color::Blue);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                m_isDone = setScene<BattleField>();

                return;
            }
        }
        else
        {
            text.setColor(sf::Color::Red);
        }
    };

    change_color(m_menuItems[0], point);
    change_color(m_menuItems[1], point);
    change_color(m_menuItems[2], point);
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isDone)
        return;
        
    target.draw(m_menuItems[0]);
    target.draw(m_menuItems[1]);
    target.draw(m_menuItems[2]);
}