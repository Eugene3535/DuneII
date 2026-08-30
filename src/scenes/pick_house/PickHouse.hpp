#pragma once

#include "common/Enums.hpp"
#include "graphics/sprites/SpriteManager.hpp"
#include "scenes/Scene.hpp"


class PickHouse:
    public Scene
{
public:
    explicit PickHouse(struct Game* game) noexcept;
    ~PickHouse();

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw(const mat4s& projection) noexcept override;
    void resize(int width, int height) noexcept override;

private:
    uint32_t m_vertexBufferObject;
    uint32_t m_vertexArrayObjects[2];

    Background m_background;
    Transform2D m_backgroundTransform;

    Outline m_outline;
    Transform2D m_outlineTransform;

    HouseType m_selectedHouse;
    float m_timer;
    bool m_outlineNeedUpdate;
};