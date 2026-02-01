#ifndef CONSTRUCTION_MENU_HPP
#define CONSTRUCTION_MENU_HPP

#include <vector>

#include "graphics/transform/Transform2D.hpp"

class ConstructionMenu
{
public:
    enum Preview : uint32_t
    {
        WOR = 0,
        Wind_Trap,
        Wall,
        Death_Hand,
        Quad,
        Harvester,
        Light_Vehicle_Factory,
        Starport,
        Turret,
        Slab_2x2,
        Raider_Trike,
        MCV,
        House_of_IX,
        Spice_Silo,
        Rocket_Turret,
        Construction_Yard,
        Launcher,
        Carryall,
        High_Tech,
        Repair,
        Refinery,
        Barracks,
        Siege_Tank,
        Starport_Intro,
        Heavy_Vehicle_Factory,
        Palace,
        Outpost,
        Fremen,
        Sonic_Tank,
        Devastator,
        Saboteur,
        Sandworm,
        Trooper,
        Troopers,
        Trike,
        Deviator,
        Ornithopter,
        Infantry,
        Tank,
        Empty_Cell,
        MAX,
        INVALID = 0xFF
    };

    ConstructionMenu(const class Engine* engine)           noexcept;
    ConstructionMenu(const ConstructionMenu&)              noexcept = delete;
	ConstructionMenu(ConstructionMenu&&)                   noexcept = delete;
	ConstructionMenu& operator = (const ConstructionMenu&) noexcept = delete;
	ConstructionMenu& operator = (ConstructionMenu&&)      noexcept = delete;
    ~ConstructionMenu();

    void init()         noexcept;
    void showEntityInfo(Preview preview) noexcept;
    void showEntityMenu(Preview preview)  noexcept;
    void hide()         noexcept;
    void draw()   const noexcept;
    void resize(int width, int height) noexcept;

    bool isShown() const noexcept;
    const Transform2D& getTransform() const noexcept;

private:
    void createFrame()    noexcept;
    void createPreviews() noexcept;
    void drawFrame()      const noexcept;
    void drawPreviews()   const noexcept;

    Transform2D m_transform;

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