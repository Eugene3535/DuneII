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
    void showMenuForEntity() noexcept;
    void draw() noexcept;

private:
    uint32_t m_vao;
    uint32_t m_vbo;

    bool m_isEnabled;
};

#endif // !CONSTRUCTION_MENU_HPP