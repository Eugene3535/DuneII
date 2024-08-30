#include <limits>
#include <thread>

#include "effects/ScreenBlackoutEffect.hpp"

ScreenBlackoutEffect::ScreenBlackoutEffect() noexcept:
    m_alpha(0),
    m_isOver(true)
{
    auto video_mode = sf::VideoMode::getDesktopMode();
    prepare({ 0.0f, 0.0f }, { video_mode.width, video_mode.height });
}

ScreenBlackoutEffect::~ScreenBlackoutEffect()
{
}

void ScreenBlackoutEffect::prepare(const sf::Vector2f& view_center, const sf::Vector2u& window_size) noexcept
{
    m_blackoutShape.setPosition(view_center.x - (window_size.x >> 1), view_center.y - (window_size.y >> 1));
    m_blackoutShape.setSize(sf::Vector2f(window_size.x, window_size.y));
    m_alpha = 0;
    m_blackoutShape.setFillColor(sf::Color(0, 0, 0, m_alpha));
    m_isOver = false;
}

void ScreenBlackoutEffect::update() noexcept
{  
    if(m_isOver)
        return;

    constexpr uint32_t timeout = 210;
    constexpr uint32_t step = 5;
 
    m_blackoutShape.setFillColor(sf::Color(0, 0, 0, m_alpha));
    m_alpha += step;

    if(m_alpha >= timeout)
        m_isOver = true;
}

bool ScreenBlackoutEffect::isOver() const noexcept
{
    return m_isOver;
}

void ScreenBlackoutEffect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_blackoutShape, states); 
}