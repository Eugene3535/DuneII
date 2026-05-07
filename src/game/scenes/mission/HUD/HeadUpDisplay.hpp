#ifndef HEAD_UP_DISPLAY_HPP
#define HEAD_UP_DISPLAY_HPP

#include <entt/entity/fwd.hpp>

#include "common/NonCopyable.hpp"
#include "common/NonMovable.hpp"
#include "graphics/Sprite2D.hpp"
#include "graphics/transform/Transform2D.hpp"
#include "graphics/sprites/SpriteManager.hpp"
#include "game/scenes/mission/HUD/previews/EntityPreview.hpp"


class HeadUpDisplay: 
    private NonCopyable,
    private NonMovable
{
public:
    HeadUpDisplay(class Engine* engine, class Tilemap& tilemap, class ConstructionMenu& menu) noexcept;
    ~HeadUpDisplay();

    bool init()                              noexcept;
    void update(float dt)                    noexcept;
    void draw(const mat4s& projection) const noexcept;

    void runSelection()                noexcept;
    void cancelSelection()             noexcept;
    void resize(int width, int height) noexcept;

    bool isEntitySelected() const noexcept;
    entt::entity getLastSelectedEntity() const noexcept;

    uint32_t getTexture() const noexcept;

private:
    class Engine*           m_engine;
    const class Tilemap&    m_tilemap;
    class ConstructionMenu& m_menu;
    EntityPreview           m_previewIcon;

    uint32_t m_tilemapProgram;
    uint32_t m_previewTexture;

    SpriteManager m_sprites;
    Sprite2D      m_releasedCursor;
    Sprite2D      m_capturedCursor;
    Sprite2D      m_currentCursor;

    struct
    {
        Transform2D transform;
        uint32_t    texture;
        uint32_t    program;
        float       timer;
    } m_cursor;

    struct
    {
        uint32_t     vertexBufferObject;
        uint32_t     vertexArrayObject;
        float        blinkTimer;
        entt::entity lastSelectedEntity;
    } m_selectionFrame;
};

#endif // !HEAD_UP_DISPLAY_HPP