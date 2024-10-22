#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#include "ui/Cursor.hpp"
#include "ecs/components/Structure.hpp"
#include "scenes/mission/tilemap/TileMap.hpp"
#include "ecs/systems/CursorController.hpp"

CursorController::CursorController(entt::registry& registry, sf::RenderWindow& rw, Cursor& cursor, TileMap& map) noexcept :
    System(registry),
    m_window(rw),
    m_cursor(cursor),
    m_tilemap(map),
    m_timer(0)
{
    m_window.setMouseCursorVisible(false);
}

CursorController::~CursorController()
{
    m_window.setMouseCursorVisible(true);
}

void CursorController::execute(sf::Time dt) noexcept
{
    static constexpr int32_t cooldown = 4;

    sf::Vector2i mouse_position  = sf::Mouse::getPosition(m_window);
    auto world_position = m_window.mapPixelToCoords(mouse_position);

    m_cursor.setPosition(world_position);

    if(m_timer > cooldown)
    {
        m_timer = 0;

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(auto entity = m_tilemap.getEntityUnderCursor(static_cast<sf::Vector2i>(world_position)); entity.has_value())
            {
                if(auto [structure, bounds] = m_registry.try_get<Structure, sf::IntRect>(entity.value()); structure != nullptr)
                {
                    bool can_be_highlighted = 
                                ((structure->type != StructureType::SLAB_1x1) &&
                                ( structure->type != StructureType::SLAB_2x2) && 
                                ( structure->type != StructureType::WALL)     && 
                                  structure->type < StructureType::MAX);

                    if(can_be_highlighted)
                    {
                        m_cursor.setVertexFrame(*bounds);
                        m_cursor.select();
                    }
                    else
                    {
                        m_cursor.release();
                    }
                }
            }
            else
            {
                m_cursor.release();
            }

            //m_cursor.capture(); // for units
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            m_cursor.release();
        }
    }

    ++m_timer;
}
