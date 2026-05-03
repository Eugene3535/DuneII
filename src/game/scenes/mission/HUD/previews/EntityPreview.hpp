#ifndef ENTITY_PREVIEW_HPP
#define ENTITY_PREVIEW_HPP

#include <vector>
#include <span>
#include <filesystem>

#include <cglm/struct/ivec2.h>



class EntityPreview
{
public:
    enum Icon : uint32_t
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

    EntityPreview(class Engine* engine) noexcept;
    ~EntityPreview();

    bool loadFromFile(const std::filesystem::path& filepath) noexcept;
    void createIcon(const ivec2s position, const ivec2s size) noexcept;
    void setIcon(Icon icon) noexcept;

    void draw() const noexcept;
    void draw(EntityPreview::Icon icon, float progress) const noexcept;

    std::span<const vec2s> getTexCoords(Icon icon) const noexcept;

private:
    class Engine* m_engine;

    uint32_t m_texture;
    uint32_t m_defaultProgram;
    uint32_t m_progressProgram;
    uint32_t m_vertexBufferObject;
    uint32_t m_vertexArrayObject;

    struct
    {
        int32_t top;
        int32_t bottom;
        int32_t progress;
    } m_uniforms;

    std::vector<vec2s> m_textureGrid;
};

#endif // !ENTITY_PREVIEW_HPP