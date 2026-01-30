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
    HeadUpDisplay(const class Engine* engine, const class Builder& builder) noexcept;
    HeadUpDisplay(const HeadUpDisplay&)              noexcept = delete;
	HeadUpDisplay(HeadUpDisplay&&)                   noexcept = delete;
	HeadUpDisplay& operator = (const HeadUpDisplay&) noexcept = delete;
	HeadUpDisplay& operator = (HeadUpDisplay&&)      noexcept = delete;
    ~HeadUpDisplay();

    bool init() noexcept;
    void update(const Transform2D& sceneTransform, float dt) noexcept;

    void drag()     noexcept;
    void drop()     noexcept;
    void showMenu() noexcept;
    void hideMenu() noexcept;

    void drawSelection() const noexcept;
    void drawCursor()    const noexcept;
    void drawMenu()      const noexcept;

    void resize(int width, int height) noexcept;

    bool isSelectionEnabled() const noexcept;
    bool isMenuShown()        const noexcept;

    const Transform2D& getCursorTransform() const noexcept;
    const Transform2D& getMenuTransform()   const noexcept;

private:
    const class Engine*  m_engine; 
    const class Builder& m_builder;
    ConstructionMenu     m_menu;

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

    bool m_isCaptured;
};

#endif // !HEAD_UP_DISPLAY_HPP