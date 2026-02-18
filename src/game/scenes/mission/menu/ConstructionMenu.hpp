#ifndef CONSTRUCTION_MENU_HPP
#define CONSTRUCTION_MENU_HPP

#include <vector>
#include <span>

#include "common/Enums.hpp"
#include "graphics/transform/Transform2D.hpp"


class ConstructionMenu
{
public:
    ConstructionMenu(const class Engine* engine, class Tilemap& tilemap) noexcept;
    ConstructionMenu(const ConstructionMenu&)              noexcept = delete;
	ConstructionMenu(ConstructionMenu&&)                   noexcept = delete;
	ConstructionMenu& operator = (const ConstructionMenu&) noexcept = delete;
	ConstructionMenu& operator = (ConstructionMenu&&)      noexcept = delete;
    ~ConstructionMenu();

    void init()                              noexcept;
    void showEntityInfo(PreviewType preview) noexcept;
    void showEntityMenu(PreviewType mainPreview, std::span<PreviewType> menu) noexcept;
    void hide()                              noexcept;
    void draw()                        const noexcept;
    void resize(int width, int height)       noexcept;

    bool isShown() const noexcept;
    const Transform2D& getTransform() const noexcept;

private:
    void createFrame()    noexcept;
    void createPreviews() noexcept;
    void drawFrame()      const noexcept;
    void drawPreviews()   const noexcept;

    const class Engine* m_engine;
    class Tilemap&      m_tilemap;
    Transform2D         m_transform;

    struct Widget
    {
        uint32_t background;
        uint32_t outline;
    };

    struct
    {
        uint32_t vao;
        uint32_t vbo;
        uint32_t program;
        int32_t  uniform;

        Widget rootWidget;
        Widget entityWidget;
        Widget entityWidgetLabel;
        Widget entityWidgetParams[3];
    } m_frame;

    struct
    {
        uint32_t program;
        uint32_t texture;
        uint32_t vao;
        uint32_t vbo;
        uint32_t cellCount;
    } m_previews;

    std::vector<vec2s> m_textureGrid;

    bool m_isShown;
};

#endif // !CONSTRUCTION_MENU_HPP