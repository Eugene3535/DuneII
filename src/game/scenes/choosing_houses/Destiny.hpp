#ifndef DESTINY_HPP
#define DESTINY_HPP

#include <memory>

#include <SFML/Graphics/RectangleShape.hpp>

#include "scenes/Scene.hpp"

namespace sf
{
    class Sprite;
    class Music;
}

class Destiny:
    public Scene
{
public:
    explicit Destiny(class DuneII* game) noexcept;

    bool load(const std::string& info) noexcept override;
    void update(const sf::Time dt)     noexcept override;

    void resize(const sf::Vector2f& size) noexcept;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape m_outline;

    std::unique_ptr<class sf::Sprite> m_sprite;

    class sf::Music* m_theme;
};

#endif // !DESTINY_HPP