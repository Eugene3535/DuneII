#pragma once

#include <vector>
#include <span>


#include "graphics/sprites/SpriteManager.hpp"
#include "graphics/transform/Transform2D.hpp"


class ConstructionMenu
{
public:
    enum class ButtonType : uint32_t
    {
        Exit   = 0,
        Repair = 1,
        Stop   = 2,
        NotSelected = 3
    };

    ConstructionMenu(struct Game* game) noexcept;
    ~ConstructionMenu();

    void init(uint32_t texture)                                               noexcept;
    void showEntityMenu(EntityIcon mainIcon, std::span<EntityIcon> menuIcons) noexcept;
    void updateSelection(char keyCode, bool isForced = false)                 noexcept;
    void hide()                                                               noexcept;
    void draw(const mat4s& projection)                                  const noexcept;
    void resize(int width, int height)                                        noexcept;

    EntityIcon getSelectedPreview() const noexcept;
    ButtonType getSelectedButton()  const noexcept;

    bool isShown()                    const noexcept;
    const Transform2D& getTransform() const noexcept;

private:
    void createFrames()       noexcept;
    void createPreviews()     noexcept;
    void createUserElements() noexcept;

    void drawFrames()       const noexcept;
    void drawPreviews()     const noexcept;
    void drawUserElements() const noexcept;

    struct Game* m_game;
    Transform2D  m_transform;

    ConstructionMenuFrames       m_frames;
    ConstructionMenuPreviewCells m_previewCells;
    ConstructionMenuElements     m_menuElements;
    
    std::vector<vec2s> m_textureGrid;
    std::vector<EntityIcon> m_previews;
    bool m_isShown;
};