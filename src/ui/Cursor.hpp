#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <unordered_map>
#include <string>

#include <SFML/Graphics/Sprite.hpp>

class Cursor final:
    public sf::Sprite
{
public:
    Cursor() noexcept;
    ~Cursor();

    bool load(class AnimationManager& animator) noexcept;
    void capture() noexcept;
    void release() noexcept;

    bool isCaptured() const noexcept;

private:
    std::unordered_map<std::string, sf::IntRect> m_frames;
    bool m_isCaptured;
};

#endif // !CURSOR_HPP