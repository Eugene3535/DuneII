#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include "scenes/Scene.hpp"

class MainMenu:
    public Scene
{
public:
    MainMenu() noexcept;
    ~MainMenu();

    bool load(const std::string& info) noexcept override;
    void open()                        noexcept override;
    void close()                       noexcept override;
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