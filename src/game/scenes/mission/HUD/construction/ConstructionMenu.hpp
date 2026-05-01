#ifndef CONSTRUCTION_MENU_HPP
#define CONSTRUCTION_MENU_HPP

#include <vector>
#include <span>

#include "common/Enums.hpp"
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

    ConstructionMenu(class Engine* engine, class Tilemap& tilemap) noexcept;
    ~ConstructionMenu();

    void init()                                                               noexcept;
    void update(float dt)                                                     noexcept;
    void showEntityView(PreviewType preview, bool enableConstruction)         noexcept;
    void showEntityMenu(PreviewType mainPreview, std::span<PreviewType> menu) noexcept;
    void updateSelection(char keyCode, bool isForced = false)                 noexcept;
    void hide()                                                               noexcept;
    void draw(bool onlyEntityView)                                      const noexcept;
    void resize(int width, int height)                                        noexcept;

    PreviewType getSelectedPreview() const noexcept;
    ButtonType  getSelectedButton() const noexcept;
    float* getProgress() noexcept;

    bool isShown()                    const noexcept;
    const Transform2D& getTransform() const noexcept;

private:
    void createFrames()       noexcept;
    void createPreviews()     noexcept;
    void createUserElements() noexcept;

    void drawFrames()       const noexcept;
    void drawPreviews()     const noexcept;
    void drawEntityView()   const noexcept;
    void drawUserElements() const noexcept;

    class Engine*  m_engine;
    class Tilemap& m_tilemap;
    Transform2D    m_transform;

    struct Widget
    {
        uint32_t background;
        uint32_t outline;
    };

    struct
    {
        uint32_t vertexBufferObject;
        uint32_t vertexArrayObject;
        uint32_t program;
        int32_t  uniformColor;

        Widget rootWidget;
        Widget entityWidget;
        Widget entityWidgetLabel;
        Widget entityWidgetParams[3];
    } m_frames;

    struct
    {
        uint32_t program;
        uint32_t texture;
        uint32_t vertexBufferObject;
        uint32_t vertexArrayObject;
        uint32_t cellCount;
        uint32_t cellWidth;
        uint32_t cellHeight;

        struct // side bar preview
        {
            uint32_t program;
            float progress;

            struct
            {
                int32_t top;
                int32_t bottom;
                int32_t progress;
            } uniform;

        } entityPreview;

    } m_previewCells;

    struct 
    {        
        uint32_t program;
        uint32_t textures[3];
        uint32_t vertexBufferObject;
        uint32_t vertexArrayObject;
        Sprite2D buttonExit;
        Sprite2D buttonRepair;
        Sprite2D buttonStop;
        PreviewType lastSelectedPreview;
        
        struct
        {
            uint32_t vertexArrayObject;
            uint32_t program;
            uint32_t count;
            int32_t row;
            int32_t column;
            float moveFrameDelayTimer;
        } selectionFrame;

    } m_userElements;
    
    std::vector<vec2s> m_textureGrid;
    std::vector<PreviewType> m_previews;
    bool m_isShown;
};

#endif // !CONSTRUCTION_MENU_HPP