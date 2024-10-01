#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "scenes/Scene.hpp"

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
    sf::Sprite m_planet;
    sf::Text   m_startGame;
    sf::Text   m_settings;
    sf::Text   m_tutorial;
};

#endif // !TITLE_SCREEN_HPP