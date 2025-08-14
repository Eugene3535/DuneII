#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include <memory>

#include "ecs/components/Animation.hpp"
#include "scenes/Scene.hpp"


namespace sf
{
    class Sprite;
    class Text;
}


class MainMenu:
    public Scene
{
public:
    MainMenu(class DuneII* game) noexcept;
    ~MainMenu();

    bool load(const std::string& info) noexcept override;
    void update(sf::Time dt)           noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::unique_ptr<class sf::Sprite> m_titleScreen;
    Animation m_animation;

    std::unique_ptr<class sf::Text> m_startGame;
    std::unique_ptr<class sf::Text> m_settings;
    std::unique_ptr<class sf::Text> m_tutorial;
};

#endif // !MAIN_MENU_HPP