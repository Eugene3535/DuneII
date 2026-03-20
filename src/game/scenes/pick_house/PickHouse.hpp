#ifndef PICK_HOUSE_HPP
#define PICK_HOUSE_HPP

#include <memory>

#include <SFML/Graphics/RectangleShape.hpp>

#include "game/scenes/Scene.hpp"

namespace sf
{
    class Sprite;
    class Music;
}

class PickHouse:
    public Scene
{
public:
    explicit PickHouse(class DuneII* game) noexcept;

    bool load(std::string_view data) noexcept override;
    void update(sf::Time dt)         noexcept override;
    void resize(sf::Vector2u size)   noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape m_outline;

    std::unique_ptr<class sf::Sprite> m_sprite;

    class sf::Music* m_theme;
};

#endif // !PICK_HOUSE_HPP