#ifndef SCREEN_BLACKOUT_EFFECT_HPP
#define SCREEN_BLACKOUT_EFFECT_HPP

// The class is used to create a blackout effect when transitioning between scenes

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

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
    std::int32_t m_alpha;
    bool m_isOver;
};

#endif