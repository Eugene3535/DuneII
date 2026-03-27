
#include "resources/files/FileProvider.hpp"
#include "game/scenes/mission/tilemap/Tilemap.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/mission/HUD/HeadUpDisplay.hpp"

#define BLINK_PERIOD 0.125f
#define BLINK_LOOP_TIME 0.25f


HeadUpDisplay::HeadUpDisplay(DuneII* game, const Tilemap& tilemap) noexcept:
    Scene(game, Scene::HUD),
    m_tilemap(tilemap),
    // m_menu(engine, tilemap),
    m_clickTimer(0.f)
{
    m_selectionFrame.blinkTimer = 0.f;
    m_selectionFrame.enabled = false;
    m_selectionFrame.lastSelectedEntity = entt::null;
}


HeadUpDisplay::~HeadUpDisplay() = default;


bool HeadUpDisplay::load(std::string_view data) noexcept
{
    if(!m_cursor.load(m_game->animations, m_game->assets))
        return false;

    m_game->window.setMouseCursorVisible(false);

    return true;
}


void HeadUpDisplay::update(sf::Time dt) noexcept
{
    updateCursor(dt);
}


void HeadUpDisplay::resize(sf::Vector2u size) noexcept
{
    //m_menu.resize(width, height);
}


void HeadUpDisplay::hideMenu() noexcept
{
    //m_menu.hide();
}


void HeadUpDisplay::runSelection() noexcept
{
    // if(m_menu.isShown())
    //     return;

    const sf::Vector2i mousePosition = sf::Mouse::getPosition(m_game->window);
    const sf::Vector2i worldPosition = sf::Vector2i(m_game->window.mapPixelToCoords(mousePosition));

    const auto entity = m_tilemap.getEntityUnderCursor(worldPosition);

    if(entity == entt::null)
    {
        m_selectionFrame.enabled = false;
        m_selectionFrame.lastSelectedEntity = entt::null;

        return;
    }

    auto convert_building_type_to_preview = [](StructureInfo::Type type) -> PreviewType
    {
        switch (type)
        {
            case StructureInfo::SLAB_2x2:          return PreviewType::Slab_2x2;
            case StructureInfo::PALACE:            return PreviewType::Palace;
            case StructureInfo::VEHICLE:           return PreviewType::Light_Vehicle_Factory;
            case StructureInfo::HIGH_TECH:         return PreviewType::High_Tech;
            case StructureInfo::CONSTRUCTION_YARD: return PreviewType::Construction_Yard;
            case StructureInfo::WIND_TRAP:         return PreviewType::Wind_Trap;
            case StructureInfo::BARRACKS:          return PreviewType::Barracks;
            case StructureInfo::STARPORT:          return PreviewType::Starport;
            case StructureInfo::REFINERY:          return PreviewType::Refinery;
            case StructureInfo::REPAIR:            return PreviewType::Repair;
            case StructureInfo::TURRET:            return PreviewType::Turret;
            case StructureInfo::ROCKET_TURRET:     return PreviewType::Rocket_Turret;
            case StructureInfo::SILO:              return PreviewType::Spice_Silo;
            case StructureInfo::OUTPOST:           return PreviewType::Outpost;
        
            default: return PreviewType::Empty_Cell;
        }
    };

    auto& registry = m_tilemap.getRegistry();

    // if(m_selectionFrame.lastSelectedEntity != entity)
    // {
    //     m_selectionFrame.lastSelectedEntity = entity;
    // }           
    // else
    // {
    //     if(m_clickTimer > BLINK_LOOP_TIME)
    //     {
    //         if(StructureInfo* info = registry.try_get<StructureInfo>(entity))
    //         {
    //             const auto mainPreview = convert_building_type_to_preview(info->type);

    //             if(mainPreview != PreviewType::Empty_Cell)
    //             {
    //                 const bool hasConstructionPreviews = ((info->type == StructureInfo::Type::VEHICLE)           ||
    //                                                       (info->type == StructureInfo::Type::HIGH_TECH)         || 
    //                                                       (info->type == StructureInfo::Type::CONSTRUCTION_YARD) || 
    //                                                       (info->type == StructureInfo::Type::BARRACKS)          ||
    //                                                       (info->type == StructureInfo::Type::STARPORT));

    //                 std::span<PreviewType> previews;

    //                 if(hasConstructionPreviews)
    //                 {
    //                     std::vector<PreviewType>* previewArray = registry.try_get<std::vector<PreviewType>>(entity);

    //                     if(previewArray)
    //                         previews = std::span(*previewArray);
    //                 }

    //                 m_menu.showEntityMenu(mainPreview, previews);
    //             }        
    //         }     
    //     }
            
    //     return;
    // }

    m_clickTimer = 0;

    auto [info, bounds] = registry.try_get<StructureInfo, sf::IntRect>(entity);

    if(info)
    {
        bool isSelectable = ((info->type != StructureInfo::Type::SLAB_1x1) &&
                             (info->type != StructureInfo::Type::SLAB_2x2) && 
                             (info->type != StructureInfo::Type::WALL)     && 
                              info->type <  StructureInfo::Type::MAX);

        if(isSelectable)
        {
            const auto entityView = convert_building_type_to_preview(info->type);

            // if(entityView != PreviewType::Empty_Cell)
            //     m_menu.showEntityView(entityView);

            if (bounds)
            {
                m_cursor.setVertexFrame(*bounds);
                m_cursor.select();
            }
        }
    }
}


void HeadUpDisplay::cancelSelection() noexcept
{
    m_selectionFrame.enabled = false;
    m_selectionFrame.lastSelectedEntity = entt::null;
}


bool HeadUpDisplay::isMenuShown() const noexcept
{
    // return m_menu.isShown();
    return false;
}


void HeadUpDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_cursor, states);

    // auto& camera = m_engine->camera;

    // alignas(16) mat4s uniformMatrix = camera.getModelViewProjectionMatrix();
    // alignas(16) mat4s modelView     = m_tilemap.getMatrix();
    // alignas(16) mat4s result        = glms_mul(uniformMatrix, modelView);

    // if(!m_menu.isShown()) // Move viewport and draw cursor
    // {
    //     if(m_selectionFrame.enabled && m_selectionFrame.blinkTimer < BLINK_PERIOD)
    //     {
    //         glUseProgram(m_cursorProgram);
    //         glBindVertexArray(m_selectionFrame.vertexArrayObject);
    //         glDrawArrays(GL_LINES, 0, 16);

    //         glUseProgram(m_tilemapProgram); // return to default tilemap shader
    //     }
        
    //     modelView = m_cursorTransform.getMatrix();
    //     result = glms_mul(uniformMatrix, modelView);
    //     camera.updateUniformBuffer(result.raw);
        
    //     m_sprites.bind(true);
    //     glBindTextureUnit(0, m_cursorTexture);
    //     glDrawArrays(GL_TRIANGLE_FAN, m_currentCursor.frame, 4);
    //     glBindTextureUnit(0, 0);

    //     if(m_selectionFrame.enabled && (m_selectionFrame.lastSelectedEntity != entt::null))
    //     {
    //         modelView = m_menu.getTransform().getMatrix();
    //         result = glms_mul(uniformMatrix, modelView);
    //         camera.updateUniformBuffer(result.raw);
    //         m_menu.draw(true);
    //     }
    // }
    // else
    // {
    //     modelView = m_menu.getTransform().getMatrix();
    //     result = glms_mul(uniformMatrix, modelView);
    //     camera.updateUniformBuffer(result.raw);
    //     m_menu.draw(false);
    // }
}


void HeadUpDisplay::updateCursor(sf::Time dt) noexcept
{
    static constexpr int32_t cooldown = 4;
    static int timer = 0;

    sf::Vector2i mousePosition = sf::Mouse::getPosition(m_game->window);
    const auto worldPosition = m_game->window.mapPixelToCoords(mousePosition);

    m_cursor.update(worldPosition, dt);

    if(timer > cooldown)
    {
        timer = 0;

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if(auto entity = m_tilemap.getEntityUnderCursor(static_cast<sf::Vector2i>(worldPosition)); entity != entt::null)
            {
                // if(auto [structure, bounds] = mission->m_registry.try_get<Structure, sf::IntRect>(entity.value()); structure != nullptr)
                // {
                //     bool can_be_highlighted = 
                //                 ((structure->type != StructureType::SLAB_1x1) &&
                //                 ( structure->type != StructureType::SLAB_2x2) && 
                //                 ( structure->type != StructureType::WALL)     && 
                //                 structure->type < StructureType::MAX);

                //     if(can_be_highlighted)
                //     {
                //         mission->m_cursor.setVertexFrame(*bounds);
                //         mission->m_cursor.select();
                //     }
                //     else
                //     {
                //         mission->m_cursor.release();
                //     }
                // }
            }
            else
            {
                m_cursor.release();
            }

            //m_cursor.capture(); // for units
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
            m_cursor.release();
    }

    ++timer;
}