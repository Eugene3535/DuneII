#ifndef CONSTRUCTION_MENU_HPP
#define CONSTRUCTION_MENU_HPP

#include <vector>

#include "graphics/transform/Transform2D.hpp"
#include "graphics/outline/Outline.hpp"

class ConstructionMenu
{
public:
    ConstructionMenu(const ivec2s& windowSize) noexcept;
    ConstructionMenu(const ConstructionMenu&)              noexcept = delete;
	ConstructionMenu(ConstructionMenu&&)                   noexcept = delete;
	ConstructionMenu& operator = (const ConstructionMenu&) noexcept = delete;
	ConstructionMenu& operator = (ConstructionMenu&&)      noexcept = delete;
    ~ConstructionMenu();

    void init()    noexcept;
    void update()  noexcept;
    void enable()  noexcept;
    void disable() noexcept;
    void draw()    noexcept;
    void drawOutline() noexcept;

    bool isEnabled() const noexcept;
    const Transform2D& getTransform() const noexcept;

private:
    const ivec2s& m_windowSize;
    Transform2D m_transform;

    uint32_t m_vao;
    uint32_t m_vbo;
    Outline m_outline;

    bool m_isEnabled;
};

#endif // !CONSTRUCTION_MENU_HPP