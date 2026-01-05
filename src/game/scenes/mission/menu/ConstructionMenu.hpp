#ifndef CONSTRUCTION_MENU_HPP
#define CONSTRUCTION_MENU_HPP

#include <vector>


class ConstructionMenu
{
public:
    ConstructionMenu() noexcept;
    ConstructionMenu(const ConstructionMenu&)              noexcept = delete;
	ConstructionMenu(ConstructionMenu&&)                   noexcept = delete;
	ConstructionMenu& operator = (const ConstructionMenu&) noexcept = delete;
	ConstructionMenu& operator = (ConstructionMenu&&)      noexcept = delete;
    ~ConstructionMenu();

    void createMenu() noexcept;

private:
    uint32_t m_texture;
    uint32_t m_vao;
    uint32_t m_vbo;
    uint32_t m_program;
};

#endif // !CONSTRUCTION_MENU_HPP