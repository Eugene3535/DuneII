#include "animation/Animation.hpp"

Animation::Animation() noexcept:
    m_frames(nullptr), 
    m_timer(0.0f), 
    m_fps(0.0f) 
{

}

Animation::Animation(const std::vector<sf::IntRect>* frames, float rate) noexcept:
    Animation()
{
    setFrames(frames);
    setRate(rate);
}

void Animation::setFrames(const std::vector<sf::IntRect>* frames) noexcept
{
    m_frames = frames;
}

void Animation::setRate(float fps) noexcept
{
    m_fps = std::fabs(fps);
}

void Animation::restart() noexcept
{
    m_timer = 0;
}

const std::vector<sf::IntRect>* Animation::getFrames() const noexcept
{
    return m_frames;
}

float Animation::getRate() const noexcept
{
    return m_fps;
}

bool Animation::isOver() const noexcept
{
    return static_cast<size_t>(m_timer + m_fps) >= m_frames->size();
}
