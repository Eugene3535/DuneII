#ifndef CONSTRUCTION_MENU_HPP
#define CONSTRUCTION_MENU_HPP

#include <vector>

#include "graphics/transform/Transform2D.hpp"

// More info is available here
// https://gamicus.fandom.com/wiki/List_of_structures_in_Dune_II

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

    ConstructionMenu(const ivec2s& windowSize) noexcept;
    ConstructionMenu(const ConstructionMenu&)              noexcept = delete;
	ConstructionMenu(ConstructionMenu&&)                   noexcept = delete;
	ConstructionMenu& operator = (const ConstructionMenu&) noexcept = delete;
	ConstructionMenu& operator = (ConstructionMenu&&)      noexcept = delete;
    ~ConstructionMenu();

    void init(uint32_t frameProgram, uint32_t previewProgram) noexcept;
    void update()  noexcept;
    void enable()  noexcept;
    void disable() noexcept;
    void draw()    noexcept;

    bool isEnabled() const noexcept;
    const Transform2D& getTransform() const noexcept;

private:
    void createFrame(uint32_t program) noexcept;
    void createPreviews(uint32_t program) noexcept;
    void drawFrame() noexcept;
    void drawPreviews() noexcept;

    const ivec2s& m_windowSize;
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

        Widget rootWindow;
        Widget entityWindow;
        Widget entityWindowLabel;
        Widget entityWindowParams[3];
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

    bool m_isEnabled;
};

#endif // !CONSTRUCTION_MENU_HPP