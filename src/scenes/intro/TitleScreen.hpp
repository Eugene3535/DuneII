#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include <memory>

#include <SFML/Graphics/Sprite.hpp>

#include "scenes/Scene.hpp"


class TitleScreen:
    public Scene
{
public:
    TitleScreen(class DuneII* game) noexcept;

    bool load(const std::string& info) noexcept override;
    void update(const sf::Time dt)     noexcept override;

private:
    std::unique_ptr<sf::Sprite> m_space;
    std::unique_ptr<sf::Sprite> m_planet;
    std::unique_ptr<sf::Sprite> m_buttonPlay;
    std::unique_ptr<sf::Sprite> m_buttonExit;
    std::unique_ptr<sf::Sprite> m_buttonSettings;
};

#endif // !TITLE_SCREEN_HPP