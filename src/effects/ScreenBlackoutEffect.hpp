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

    void prepare() noexcept;
    void apply() noexcept;

    bool isOver() const noexcept;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape m_blackout_shape;
    bool m_is_over;
};

#endif