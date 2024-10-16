#ifndef SCREEN_BLACKOUT_EFFECT_HPP
#define SCREEN_BLACKOUT_EFFECT_HPP

#include <cstdint>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

// The class is used to create a blackout effect when transitioning between scenes
class ScreenBlackoutEffect:
    public sf::Drawable,
    private sf::NonCopyable
{
public:
    ScreenBlackoutEffect() noexcept;
    ~ScreenBlackoutEffect();

    void prepare(const sf::Vector2f& view_center, const sf::Vector2u& window_size) noexcept;
    void update() noexcept;

    bool isOver() const noexcept;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape m_blackoutShape;
    int32_t m_alpha;
    bool m_isOver;
};

#endif