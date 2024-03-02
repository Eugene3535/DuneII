#include <limits>
#include <thread>

#include "effects/ScreenBlackoutEffect.hpp"

ScreenBlackoutEffect::ScreenBlackoutEffect() noexcept:
    m_is_over(true)
{
    auto video_mode = sf::VideoMode::getDesktopMode();
    prepare({ 0.0f, 0.0f }, { video_mode.width, video_mode.height });
}

ScreenBlackoutEffect::~ScreenBlackoutEffect()
{
}

void ScreenBlackoutEffect::prepare(const sf::Vector2f& view_center, const sf::Vector2u& window_size) noexcept
{
    m_blackout_shape.setPosition(view_center.x - (window_size.x >> 1), view_center.y - (window_size.y >> 1));
    m_blackout_shape.setSize(sf::Vector2f(window_size.x, window_size.y));
    m_blackout_shape.setFillColor(sf::Color(0, 0, 0, 0));
    m_is_over = false;
}

void ScreenBlackoutEffect::apply() noexcept
{  
    if(m_is_over)
        return;

    constexpr std::uint32_t timeout = static_cast<std::uint32_t>(std::numeric_limits<std::uint8_t>::max()) / 10 * 9; // 10% visibility in the residue
    constexpr auto delay = std::chrono::milliseconds(1000 / timeout);

    for (std::uint32_t alpha = 0; alpha < timeout; alpha += 4)
    {
        sf::Color blackout(0, 0, 0, alpha);   
        m_blackout_shape.setFillColor(blackout);
        std::this_thread::sleep_for(delay);
    }

    m_is_over = true;
}

bool ScreenBlackoutEffect::isOver() const noexcept
{
    return m_is_over;
}

void ScreenBlackoutEffect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_blackout_shape, states); 
}