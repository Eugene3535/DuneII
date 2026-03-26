
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
    if(!cursor.load(m_game->animations, m_game->assets))
        return false;

    m_game->window.setMouseCursorVisible(false);

    return true;
}


void HeadUpDisplay::update(sf::Time dt) noexcept
{
    // m_clickTimer += dt;
    // m_selectionFrame.blinkTimer += dt;

    // if(m_selectionFrame.blinkTimer > BLINK_LOOP_TIME)
    //     m_selectionFrame.blinkTimer = 0.f;

    // m_cursorTransform.setPosition(m_engine->getCursorPosition());
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

    // vec2s cursorPosition = m_engine->getCursorPosition();
    // vec2s scenePosition  = glms_vec2_negate(m_tilemap.getPosition());
    // vec2s worldCoords    = glms_vec2_add(scenePosition, cursorPosition);

    // const auto entity = m_tilemap.getEntityUnderCursor(worldCoords);

    // if(entity == entt::null)
    // {
    //     m_selectionFrame.enabled = false;
    //     m_selectionFrame.lastSelectedEntity = entt::null;

    //     return;
    // }

    // auto convert_building_type_to_preview = [](StructureInfo::Type type) -> PreviewType
    // {
    //     switch (type)
    //     {
    //         case StructureInfo::SLAB_2x2:          return PreviewType::Slab_2x2;
    //         case StructureInfo::PALACE:            return PreviewType::Palace;
    //         case StructureInfo::VEHICLE:           return PreviewType::Light_Vehicle_Factory;
    //         case StructureInfo::HIGH_TECH:         return PreviewType::High_Tech;
    //         case StructureInfo::CONSTRUCTION_YARD: return PreviewType::Construction_Yard;
    //         case StructureInfo::WIND_TRAP:         return PreviewType::Wind_Trap;
    //         case StructureInfo::BARRACKS:          return PreviewType::Barracks;
    //         case StructureInfo::STARPORT:          return PreviewType::Starport;
    //         case StructureInfo::REFINERY:          return PreviewType::Refinery;
    //         case StructureInfo::REPAIR:            return PreviewType::Repair;
    //         case StructureInfo::TURRET:            return PreviewType::Turret;
    //         case StructureInfo::ROCKET_TURRET:     return PreviewType::Rocket_Turret;
    //         case StructureInfo::SILO:              return PreviewType::Spice_Silo;
    //         case StructureInfo::OUTPOST:           return PreviewType::Outpost;
        
    //         default: return PreviewType::Empty_Cell;
    //     }
    // };

    // auto& registry = m_tilemap.getRegistry();

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

    // m_clickTimer = 0;

    // if(StructureInfo* info = registry.try_get<StructureInfo>(entity))
    // {
    //     bool isSelectable = ((info->type != StructureInfo::Type::SLAB_1x1) &&
    //                          (info->type != StructureInfo::Type::SLAB_2x2) && 
    //                          (info->type != StructureInfo::Type::WALL)     && 
    //                           info->type <  StructureInfo::Type::MAX);

    //     if(isSelectable)
    //     {
    //         const auto entityView = convert_building_type_to_preview(info->type);

    //         if(entityView != PreviewType::Empty_Cell)
    //             m_menu.showEntityView(entityView);

    //         const auto bounds = registry.get<ivec4s>(entity);
    //         glBindBuffer(GL_ARRAY_BUFFER, m_selectionFrame.vertexBufferObject);

    //         if(void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))
    //         {
    //             vec2s* vertices = static_cast<vec2s*>(data);
    //             const float offset = 12.f;

    //             const vec2s leftBottom  = { static_cast<float>(bounds.x), static_cast<float>(bounds.w) };
    //             const vec2s leftTop     = { static_cast<float>(bounds.x), static_cast<float>(bounds.y) };
    //             const vec2s rightTop    = { static_cast<float>(bounds.z), static_cast<float>(bounds.y) };
    //             const vec2s rightBottom = { static_cast<float>(bounds.z), static_cast<float>(bounds.w) };

    //             vertices[0]  = { leftBottom.x, leftBottom.y - offset };
    //             vertices[1]  = leftBottom;
    //             vertices[2]  = leftBottom;
    //             vertices[3]  = { leftBottom.x + offset, leftBottom.y };

    //             vertices[4]  = { leftTop.x, leftTop.y + offset };
    //             vertices[5]  = leftTop;
    //             vertices[6]  = leftTop;
    //             vertices[7]  = { leftTop.x + offset, leftTop.y };

    //             vertices[8]  = { rightTop.x - offset, rightTop.y };
    //             vertices[9]  = rightTop;
    //             vertices[10] = rightTop;
    //             vertices[11] = { rightTop.x, rightTop.y + offset };

    //             vertices[12] = { rightBottom.x, rightBottom.y - offset };
    //             vertices[13] = rightBottom;
    //             vertices[14] = rightBottom;
    //             vertices[15] = { rightBottom.x - offset, rightBottom.y };

    //             if(glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE)
    //                 m_selectionFrame.enabled = true;
    //         }

    //         glBindBuffer(GL_ARRAY_BUFFER, 0);
    //     }
    // }
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
    target.draw(cursor, states);

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