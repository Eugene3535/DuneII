#ifndef PICK_HOUSE_HPP
#define PICK_HOUSE_HPP

#include "common/Enums.hpp"
#include "graphics/Meshes.hpp"
#include "game/scenes/Scene.hpp"


class PickHouse:
    public Scene
{
public:
    explicit PickHouse(class Engine* engine) noexcept;
    ~PickHouse();

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw()                        noexcept override;
    void resize(int width, int height) noexcept override;

private:
    uint32_t m_vertexBufferObject;
    uint32_t m_vertexArrayObjects[2];

    struct
    {
        uint32_t     vertexArrayObject;
        mesh::Sprite sprite;
        uint32_t     program;
        Transform2D  transform;
    } m_background;
    
    struct
    {
        uint32_t vertexArrayObject;
        uint32_t program;
        uint32_t count;
        Transform2D transform;
    } m_outline;

    HouseType m_selectedHouse;
    float m_timer;
    bool m_outlineNeedUpdate;
};

#endif // !PICK_HOUSE_HPP