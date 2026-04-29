#ifndef HEAD_UP_DISPLAY_HPP
#define HEAD_UP_DISPLAY_HPP

#include <entt/entity/fwd.hpp>

#include "common/NonCopyable.hpp"
#include "common/NonMovable.hpp"
#include "graphics/Sprite2D.hpp"
#include "graphics/transform/Transform2D.hpp"
#include "graphics/sprites/SpriteManager.hpp"
#include "game/scenes/mission/HUD/construction/ConstructionMenu.hpp"


class HeadUpDisplay: 
    private NonCopyable,
    private NonMovable
{
public:
    HeadUpDisplay(class Engine* engine, class Tilemap& tilemap) noexcept;
    ~HeadUpDisplay();

    bool init()                              noexcept;
    void update(float dt)                    noexcept;
    void draw(const mat4s& projection) const noexcept;

    void runSelection()                noexcept;
    void cancelSelection()             noexcept;
    void resize(int width, int height) noexcept;

    ConstructionMenu menu;

private:
    class Engine*        m_engine;
    const class Tilemap& m_tilemap;

    enum class ClickState: uint32_t
    {
        Released,
        FirstClick,
        SecondClick
    } m_clickState;

    SpriteManager m_sprites;
    Sprite2D      m_releasedCursor;
    Sprite2D      m_capturedCursor;
    Sprite2D      m_currentCursor;
    Transform2D   m_cursorTransform;
    uint32_t      m_cursorTexture;
    uint32_t      m_cursorProgram;
    uint32_t      m_tilemapProgram;

    float m_clickTimer;

    struct
    {
        uint32_t     vertexBufferObject;
        uint32_t     vertexArrayObject;
        float        blinkTimer;
        bool         enabled;
        entt::entity lastSelectedEntity;
    } m_selectionFrame;
};

#endif // !HEAD_UP_DISPLAY_HPP