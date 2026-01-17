#ifndef CONSTRUCTION_MENU_HPP
#define CONSTRUCTION_MENU_HPP

#include <vector>

#include "graphics/transform/Transform2D.hpp"

class ConstructionMenu
{
public:
    ConstructionMenu(const ivec2s& windowSize) noexcept;
    ConstructionMenu(const ConstructionMenu&)              noexcept = delete;
	ConstructionMenu(ConstructionMenu&&)                   noexcept = delete;
	ConstructionMenu& operator = (const ConstructionMenu&) noexcept = delete;
	ConstructionMenu& operator = (ConstructionMenu&&)      noexcept = delete;
    ~ConstructionMenu();

    void init(uint32_t program) noexcept;
    void update()  noexcept;
    void enable()  noexcept;
    void disable() noexcept;
    void draw()    noexcept;

    bool isEnabled() const noexcept;
    const Transform2D& getTransform() const noexcept;

private:
    const ivec2s& m_windowSize;
    Transform2D m_transform;

    uint32_t m_vao;
    uint32_t m_vbo;
    uint32_t m_program;
    int32_t m_uniform;

    struct
    {
        uint32_t background;
        uint32_t outline;
    } m_rootWindow;

    struct
    {
        uint32_t background;
        uint32_t outline;
    } m_entityWindow;

    struct
    {
        uint32_t background;
        uint32_t outline;
    } m_entityWindowLabel;

    struct
    {
        uint32_t background;
        uint32_t outline;
    } m_entityWindowParams[3];

    bool m_isEnabled;
};

#endif // !CONSTRUCTION_MENU_HPP