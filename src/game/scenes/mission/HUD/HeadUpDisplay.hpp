#ifndef HEAD_UP_DISPLAY_HPP
#define HEAD_UP_DISPLAY_HPP

#include <span>
#include <functional>

#include <entt/entity/fwd.hpp>

#include "graphics/Meshes.hpp"
#include "graphics/transform/Transform2D.hpp"
#include "graphics/sprites/SpriteManager.hpp"


class HeadUpDisplay
{
public:
    HeadUpDisplay(const class Engine* engine, const class Builder& builder) noexcept;
    HeadUpDisplay(const HeadUpDisplay&)              noexcept = delete;
	HeadUpDisplay(HeadUpDisplay&&)                   noexcept = delete;
	HeadUpDisplay& operator = (const HeadUpDisplay&) noexcept = delete;
	HeadUpDisplay& operator = (HeadUpDisplay&&)      noexcept = delete;
    ~HeadUpDisplay();

    bool init(const std::function<void(const entt::entity)>& callback) noexcept;
    void update(const Transform2D& sceneTransform, float dt) noexcept;

    void select()  noexcept;
    void release() noexcept;

    void drawSelection() const noexcept;
    void drawCursor()    const noexcept;

    bool isSelectionEnabled() const noexcept;

    const Transform2D& getCursorTransform() const noexcept;

private:
    const class Engine*  m_engine; 
    const class Builder& m_builder;

    SpriteManager m_sprites;
    mesh::Sprite  m_releasedCursor;
    mesh::Sprite  m_capturedCursor;
    mesh::Sprite  m_currentCursor;
    Transform2D   m_cursorTransform;
    uint32_t      m_cursorTexture;
    uint32_t      m_program;

    struct
    {
        uint32_t vbo;
        uint32_t vao;
        float timer;
        bool enabled;
        entt::entity lastSelectedEntity;
    } m_selectionFrame;

    std::function<void(const entt::entity)> m_showMenuForEntity;

    bool m_isClicked;
};

#endif // !HEAD_UP_DISPLAY_HPP