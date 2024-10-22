#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <unordered_map>
#include <string>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace sf
{
    class RenderTarget;
} 


class Cursor final:
    public sf::Drawable
{
public:
    Cursor() noexcept;
    ~Cursor();

    bool load(class AnimationManager& animator) noexcept;
    void select() noexcept;
    void capture() noexcept;
    void release() noexcept;

    void update(const sf::Vector2f& position, sf::Time dt) noexcept;
    void setVertexFrame(const sf::IntRect& frame) noexcept;

    bool isSelected() const noexcept;
    bool isCaptured() const noexcept;

private:
    void draw(class sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite m_sprite;
    sf::VertexArray m_vertexFrame;

    std::unordered_map<std::string, sf::IntRect> m_frames;

    bool m_isCaptured;
    bool m_isSelected;

    sf::Time m_tick;
};

#endif // !CURSOR_HPP