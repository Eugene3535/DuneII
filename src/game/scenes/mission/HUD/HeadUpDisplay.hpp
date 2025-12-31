#ifndef HEAD_UP_DISPLAY_HPP
#define HEAD_UP_DISPLAY_HPP

#include <span>

#include "graphics/Meshes.hpp"
#include "graphics/transform/Transform2D.hpp"


class HeadUpDisplay
{
public:
    HeadUpDisplay(class Builder& builder) noexcept;
    ~HeadUpDisplay();

    void init(std::span<const Sprite> crosshairs) noexcept;

    void update(const vec2s position, float dt, bool isClicked) noexcept;
    void drawSelection() const noexcept;
    void drawCursor() const noexcept;

    const Transform2D& getCursorTransform() const noexcept;

private:
    class Builder& m_builder;

    Sprite m_releasedCursor;
    Sprite m_capturedCursor;
    Sprite* m_currentCursor;
    Transform2D m_cursorTransform;

    struct
    {
        uint32_t vbo;
        uint32_t vao;
        float timer;
        bool enabled;
    } m_selectionFrame;
    
};

#endif // !HEAD_UP_DISPLAY_HPP