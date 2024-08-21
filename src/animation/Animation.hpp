#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>

#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation() noexcept;
    Animation(const std::vector<sf::IntRect>* frames, float rate) noexcept;

    void setFrames(const std::vector<sf::IntRect>* frames) noexcept;
    void setRate(float fps) noexcept;
    void restart() noexcept;

    template<class Drawable>
    void update(Drawable* object, float dt /* delta time in seconds */) noexcept
    {
        if (m_frames)
        {
            m_timer += dt * m_fps;

            if (isOver())
                restart();

            object.setTextureRect((*m_frames)[static_cast<std::size_t>(m_timer)]);
        }
    }

    const std::vector<sf::IntRect>* getFrames() const noexcept;
    float                           getRate()   const noexcept;
    bool                            isOver()    const noexcept;

private:
    const std::vector<sf::IntRect>* m_frames;
    float                           m_timer;
    float                           m_fps;
};

#endif // !ANIMATION_HPP