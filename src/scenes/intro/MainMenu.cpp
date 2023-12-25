#include "utils/Resources.hpp"
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
    sf::Texture* texture = GetTexture("red");

    if(!texture)
        return false;

    m_buttons[0].setTexture(*texture);
    m_buttons[1].setTexture(*texture);
    m_buttons[2].setTexture(*texture);

    m_buttons[0].setPosition(250, 100);
    m_buttons[1].setPosition(250, 250);
    m_buttons[2].setPosition(250, 400);

    m_buttons[0].setScale(6, 2);
    m_buttons[1].setScale(6, 2);
    m_buttons[2].setScale(6, 2);

    return true;
}

void MainMenu::update(float dt) noexcept
{
    if(m_isDone)
        return;

    sf::Vector2f point(m_root->getCursorPosition());

    auto change_color = [this](sf::Sprite& sprite, const sf::Vector2f& point)
    {
        if(sprite.getGlobalBounds().contains(point))
        {
            sprite.setColor(sf::Color::Blue);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                m_isDone = true;
                m_root->pushScene<BattleField>();

                return;
            }
        }
        else
        {
            sprite.setColor(sf::Color::Red);
        }
    };

    change_color(m_buttons[0], point);
    change_color(m_buttons[1], point);
    change_color(m_buttons[2], point);
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(m_isDone)
        return;
        
    target.draw(m_buttons[0]);
    target.draw(m_buttons[1]);
    target.draw(m_buttons[2]);
}