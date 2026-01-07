#ifndef CONSTRUCTION_MENU_HPP
#define CONSTRUCTION_MENU_HPP

#include <vector>

#include "graphics/transform/Transform2D.hpp"

class ConstructionMenu
{
public:
    ConstructionMenu() noexcept;
    ConstructionMenu(const ConstructionMenu&)              noexcept = delete;
	ConstructionMenu(ConstructionMenu&&)                   noexcept = delete;
	ConstructionMenu& operator = (const ConstructionMenu&) noexcept = delete;
	ConstructionMenu& operator = (ConstructionMenu&&)      noexcept = delete;
    ~ConstructionMenu();

    void init()    noexcept;
    void enable()  noexcept;
    void disable() noexcept;
    void draw()    noexcept;

    bool isEnabled() const noexcept;

private:
    uint32_t m_vao;
    uint32_t m_vbo;

    bool m_isEnabled;
};

#endif // !CONSTRUCTION_MENU_HPP