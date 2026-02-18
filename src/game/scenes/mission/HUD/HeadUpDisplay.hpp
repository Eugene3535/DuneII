#ifndef HEAD_UP_DISPLAY_HPP
#define HEAD_UP_DISPLAY_HPP

#include <entt/entity/fwd.hpp>

#include "graphics/Meshes.hpp"
#include "graphics/transform/Transform2D.hpp"
#include "graphics/sprites/SpriteManager.hpp"
#include "game/scenes/mission/menu/ConstructionMenu.hpp"


class HeadUpDisplay
{
public:
    HeadUpDisplay(class Engine* engine, class Tilemap& tilemap) noexcept;
    HeadUpDisplay(const HeadUpDisplay&)              noexcept = delete;
	HeadUpDisplay(HeadUpDisplay&&)                   noexcept = delete;
	HeadUpDisplay& operator = (const HeadUpDisplay&) noexcept = delete;
	HeadUpDisplay& operator = (HeadUpDisplay&&)      noexcept = delete;
    ~HeadUpDisplay();

    bool init() noexcept;
    void update(float dt) noexcept;

    void runSelection()    noexcept;
    void cancelSelection() noexcept;
    void hideMenu()        noexcept;

    void drawSelection() const noexcept;
    void drawCursor()    const noexcept;
    void drawMenu()      const noexcept;

    void resize(int width, int height) noexcept;

    bool isSelectionEnabled() const noexcept;
    bool isMenuShown()        const noexcept;

    const Transform2D& getCursorTransform() const noexcept;
    const Transform2D& getMenuTransform()   const noexcept;

private:
    class Engine*        m_engine;
    const class Tilemap& m_tilemap;
    ConstructionMenu     m_menu;

    SpriteManager m_sprites;
    mesh::Sprite  m_releasedCursor;
    mesh::Sprite  m_capturedCursor;
    mesh::Sprite  m_currentCursor;
    Transform2D   m_cursorTransform;
    uint32_t      m_cursorTexture;
    uint32_t      m_cursorProgram;
    uint32_t      m_tilemapProgram;

    float m_clickTimer;

    struct
    {
        uint32_t vbo;
        uint32_t vao;
        float blinkTimer;
        bool enabled;
        entt::entity lastSelectedEntity;
    } m_selectionFrame;
};

#endif // !HEAD_UP_DISPLAY_HPP