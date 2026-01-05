#ifndef HEAD_UP_DISPLAY_HPP
#define HEAD_UP_DISPLAY_HPP

#include <span>

#include "graphics/Meshes.hpp"
#include "graphics/transform/Transform2D.hpp"


class HeadUpDisplay
{
public:
    HeadUpDisplay(const class Builder& builder, const Transform2D& sceneTransform) noexcept;
    HeadUpDisplay(const HeadUpDisplay&)                                            noexcept = delete;
	HeadUpDisplay(HeadUpDisplay&&)                                                 noexcept = delete;
	HeadUpDisplay& operator = (const HeadUpDisplay&)                               noexcept = delete;
	HeadUpDisplay& operator = (HeadUpDisplay&&)                                    noexcept = delete;
    ~HeadUpDisplay();

    void init(std::span<const Sprite> crosshairs) noexcept;
    void update(const vec2s cursorPosition, float dt) noexcept;

    void select() noexcept;
    void release() noexcept;

    void drawSelection() const noexcept;
    void drawCursor() const noexcept;

    bool isSelectionEnabled() const noexcept;

    const Transform2D& getCursorTransform() const noexcept;

private:
    const class Builder& m_builder;
    const Transform2D& m_sceneTransform; 

    Sprite m_releasedCursor;
    Sprite m_capturedCursor;
    Sprite m_currentCursor;
    Transform2D m_cursorTransform;

    struct
    {
        uint32_t vbo;
        uint32_t vao;
        float timer;
        bool enabled;
    } m_selectionFrame;

    bool m_isClicked;
};

#endif // !HEAD_UP_DISPLAY_HPP