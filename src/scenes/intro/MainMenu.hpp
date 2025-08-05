#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include <memory>

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
    std::unique_ptr<Animation> m_titleScreen;
    std::unique_ptr<sf::Text>  m_startGame;
    std::unique_ptr<sf::Text>  m_settings;
    std::unique_ptr<sf::Text>  m_tutorial;
};

#endif // !MAIN_MENU_HPP