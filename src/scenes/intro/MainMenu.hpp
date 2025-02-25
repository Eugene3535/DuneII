#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include <SFML/Graphics/Text.hpp>

#include "ecs/components/Animation.hpp"
#include "scenes/base/Scene.hpp"

class MainMenu:
    public Scene
{
public:
    MainMenu(struct Game& game) noexcept;
    ~MainMenu();

    bool load(const std::string& info) noexcept override;
    void update(sf::Time dt)           noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    Animation m_titleScreen;
    sf::Text  m_startGame;
    sf::Text  m_settings;
    sf::Text  m_tutorial;
};

#endif // !MAIN_MENU_HPP