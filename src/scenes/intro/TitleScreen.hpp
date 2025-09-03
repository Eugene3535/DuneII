#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include <memory>

#include <SFML/Graphics/Sprite.hpp>

#include "scenes/Scene.hpp"
#include "scenes/intro/Button.hpp"


class TitleScreen:
    public Scene
{
public:
    TitleScreen(class DuneII* game) noexcept;

    bool load(const std::string& info) noexcept override;
    void update(const sf::Time dt)     noexcept override;
    void resize(const sf::Vector2f& size) noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::unique_ptr<sf::Sprite> m_space;
    std::unique_ptr<sf::Sprite> m_planet;
    std::unique_ptr<Button> m_play;
    std::unique_ptr<Button> m_exit;
    std::unique_ptr<Button> m_settings;

    bool m_isPresented;
};

#endif // !TITLE_SCREEN_HPP