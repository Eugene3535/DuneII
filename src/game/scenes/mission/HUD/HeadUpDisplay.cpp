#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "cglm/struct/affine-mat.h"

#include "resources/files/FileProvider.hpp"
#include "resources/gl_interfaces/texture/Texture2D.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "game/scenes/mission/tilemap/Tilemap.hpp"
#include "game/Engine.hpp"
#include "game/scenes/mission/HUD/construction/ConstructionMenu.hpp"
#include "game/scenes/mission/HUD/HeadUpDisplay.hpp"

// The flicker period of the selection frame
#define BLINK_PERIOD 0.125f
#define BLINK_LOOP_TIME 0.25f


HeadUpDisplay::HeadUpDisplay(Engine* engine,  Tilemap& tilemap, ConstructionMenu& menu) noexcept:
    m_engine(engine),
    m_tilemap(tilemap),
    m_menu(menu),
    m_tilemapProgram(0),
    m_previewTexture(0),
    m_previewIcon(engine)
{
    m_cursor.texture = 0;
    m_cursor.program = 0;
    m_cursor.timer = 0;

    m_selectionFrame.vertexBufferObject = 0;
    m_selectionFrame.vertexArrayObject = 0;
    m_selectionFrame.blinkTimer = 0.f;
    m_selectionFrame.lastSelectedEntity = entt::null;
}


HeadUpDisplay::~HeadUpDisplay()
{
    glDeleteTextures(1, &m_cursor.texture);
    glDeleteTextures(1, &m_previewTexture);
    glDeleteVertexArrays(1, &m_selectionFrame.vertexArrayObject);
    glDeleteBuffers(1, &m_selectionFrame.vertexBufferObject);
}


bool HeadUpDisplay::init() noexcept
{
    m_cursor.program = m_engine->getShaderProgram("selection");
    m_tilemapProgram = m_engine->getShaderProgram("tilemap");

    if (!(m_cursor.program && m_tilemapProgram))
        return false;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_cursor.texture);
    Texture2D crosshairTexture = {.handle = m_cursor.texture };

    if (!crosshairTexture.loadFromFile(FileProvider::findPathToFile(CROSSHAIRS_TILESHEET_PNG)))
        return false;

    m_sprites.loadSpriteSheet(FileProvider::findPathToFile(CURSOR_FRAME_XML), crosshairTexture);
    auto crosshairReleased = m_sprites.getSprite("Released");
    auto crosshairCaptured = m_sprites.getSprite("Captured");

    if (!(crosshairReleased && crosshairCaptured) )
        return false;

//  Cursors
    m_releasedCursor = crosshairReleased.value();
    m_capturedCursor = crosshairCaptured.value();
    m_cursor.transform.setOrigin(m_releasedCursor.width * 0.5f, m_releasedCursor.height * 0.5f);
    m_cursor.transform.setScale(0.5f, 0.5f);
    m_currentCursor = m_releasedCursor;

//  Selection frame
    glCreateBuffers(1, &m_selectionFrame.vertexBufferObject);
	glNamedBufferData(m_selectionFrame.vertexBufferObject, sizeof(float) << 5, nullptr, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &m_selectionFrame.vertexArrayObject);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
	VertexArrayObject::createVertexInputState(m_selectionFrame.vertexArrayObject, m_selectionFrame.vertexBufferObject, attributes);

//  Entity preview
    glCreateTextures(GL_TEXTURE_2D, 1, &m_previewTexture);
    Texture2D previewTexture = {.handle = m_previewTexture };

    if (!previewTexture.loadFromFile(FileProvider::findPathToFile(PREVIEWS_PNG)))
        return false;

    if (m_previewIcon.loadFromTexture(previewTexture))
    {
        const ivec2s position = { 950, 0 };
        const ivec2s size = { 150, 100 };
        m_previewIcon.createIcon(position, size);

        return true;
    }

    return false;
}


void HeadUpDisplay::update(float dt) noexcept
{
    m_cursor.timer += dt;
    m_selectionFrame.blinkTimer += dt;

    if(m_selectionFrame.blinkTimer > BLINK_LOOP_TIME)
        m_selectionFrame.blinkTimer = 0.f;

    m_cursor.transform.setPosition(m_engine->getCursorPosition());
}


void HeadUpDisplay::draw(const mat4s& projection) const noexcept
{
    mat4s currentWorldMatrix = projection;
    mat4s modelView;
    mat4s result;

    if (m_selectionFrame.lastSelectedEntity != entt::null)
    {
        glUseProgram(m_cursor.program);

        if (m_selectionFrame.blinkTimer < BLINK_PERIOD)
        {
            glBindVertexArray(m_selectionFrame.vertexArrayObject);
            glDrawArrays(GL_LINES, 0, 16);
        }

        modelView = m_menu.getTransform().getMatrix();
        result = glms_mul(currentWorldMatrix, modelView);
        m_engine->updateUniformBuffer(result);
        m_previewIcon.draw();
    }

    if (!m_menu.isShown())
    {
        glUseProgram(m_tilemapProgram);

        modelView = m_cursor.transform.getMatrix();
        result = glms_mul(currentWorldMatrix, modelView);
        m_engine->updateUniformBuffer(result);

        m_sprites.bind(true);
        glBindTextureUnit(0, m_cursor.texture);
        glDrawArrays(GL_TRIANGLE_FAN, m_currentCursor.frame, 4);
        glBindTextureUnit(0, 0);
    }
}


void HeadUpDisplay::runSelection() noexcept
{
    if(m_menu.isShown())
        return;

    vec2s cursorPosition = m_engine->getCursorPosition();
    vec2s scenePosition  = glms_vec2_negate(m_tilemap.getPosition());
    vec2s worldCoords    = glms_vec2_add(scenePosition, cursorPosition);

    const auto entity = m_tilemap.getEntityUnderCursor(worldCoords);

    if(entity == entt::null)
    {
        cancelSelection();

        return;
    }

    auto convert_building_type_to_preview_icon = [](StructureInfo::Type type) -> EntityPreview::Icon
    {
        switch (type)
        {
            case StructureInfo::SLAB_2x2:          return EntityPreview::Icon::Slab_2x2;
            case StructureInfo::PALACE:            return EntityPreview::Icon::Palace;
            case StructureInfo::VEHICLE:           return EntityPreview::Icon::Light_Vehicle_Factory;
            case StructureInfo::HIGH_TECH:         return EntityPreview::Icon::High_Tech;
            case StructureInfo::CONSTRUCTION_YARD: return EntityPreview::Icon::Construction_Yard;
            case StructureInfo::WIND_TRAP:         return EntityPreview::Icon::Wind_Trap;
            case StructureInfo::BARRACKS:          return EntityPreview::Icon::Barracks;
            case StructureInfo::STARPORT:          return EntityPreview::Icon::Starport;
            case StructureInfo::REFINERY:          return EntityPreview::Icon::Refinery;
            case StructureInfo::REPAIR:            return EntityPreview::Icon::Repair;
            case StructureInfo::TURRET:            return EntityPreview::Icon::Turret;
            case StructureInfo::ROCKET_TURRET:     return EntityPreview::Icon::Rocket_Turret;
            case StructureInfo::SILO:              return EntityPreview::Icon::Spice_Silo;
            case StructureInfo::OUTPOST:           return EntityPreview::Icon::Outpost;

            default: return EntityPreview::Icon::Empty_Cell;
        }
    };

    auto& registry = m_tilemap.getRegistry();

    if(m_selectionFrame.lastSelectedEntity != entity)
    {
        m_selectionFrame.lastSelectedEntity = entity;
    }
    else
    {
        if(m_cursor.timer > BLINK_LOOP_TIME)
        {
            if(StructureInfo* info = registry.try_get<StructureInfo>(entity))
            {
                const auto mainPreviewIcon = convert_building_type_to_preview_icon(info->type);

                if(mainPreviewIcon != EntityPreview::Icon::Empty_Cell)
                {
                    const bool hasConstructionPreviews = ((info->type == StructureInfo::Type::VEHICLE)           ||
                                                          (info->type == StructureInfo::Type::HIGH_TECH)         ||
                                                          (info->type == StructureInfo::Type::CONSTRUCTION_YARD) ||
                                                          (info->type == StructureInfo::Type::BARRACKS)          ||
                                                          (info->type == StructureInfo::Type::STARPORT));

                    std::span<EntityPreview::Icon> previews;

                    if(hasConstructionPreviews)
                    {
                        std::vector<EntityPreview::Icon>* previewArray = registry.try_get<std::vector<EntityPreview::Icon>>(entity);

                        if(previewArray)
                            previews = std::span(*previewArray);
                    }

                    m_menu.showEntityMenu(mainPreviewIcon, previews);
                }
            }
        }

        return;
    }

    m_cursor.timer = 0;

    if(StructureInfo* info = registry.try_get<StructureInfo>(entity))
    {
        bool isSelectable = ((info->type != StructureInfo::Type::SLAB_1x1) &&
                             (info->type != StructureInfo::Type::SLAB_2x2) &&
                             (info->type != StructureInfo::Type::WALL)     &&
                              info->type <  StructureInfo::Type::MAX);

        if(isSelectable)
        {
            const auto entityIcon = convert_building_type_to_preview_icon(info->type);

            if(entityIcon != EntityPreview::Icon::Empty_Cell)
                m_previewIcon.setIcon(entityIcon);
            
            const auto bounds = registry.get<ivec4s>(entity);
            glBindBuffer(GL_ARRAY_BUFFER, m_selectionFrame.vertexBufferObject);

            if(void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))
            {
                vec2s* vertices = static_cast<vec2s*>(data);
                const float offset = 12.f;

                const vec2s leftBottom  = { static_cast<float>(bounds.x), static_cast<float>(bounds.w) };
                const vec2s leftTop     = { static_cast<float>(bounds.x), static_cast<float>(bounds.y) };
                const vec2s rightTop    = { static_cast<float>(bounds.z), static_cast<float>(bounds.y) };
                const vec2s rightBottom = { static_cast<float>(bounds.z), static_cast<float>(bounds.w) };

                vertices[0]  = { leftBottom.x, leftBottom.y - offset };
                vertices[1]  = leftBottom;
                vertices[2]  = leftBottom;
                vertices[3]  = { leftBottom.x + offset, leftBottom.y };

                vertices[4]  = { leftTop.x, leftTop.y + offset };
                vertices[5]  = leftTop;
                vertices[6]  = leftTop;
                vertices[7]  = { leftTop.x + offset, leftTop.y };

                vertices[8]  = { rightTop.x - offset, rightTop.y };
                vertices[9]  = rightTop;
                vertices[10] = rightTop;
                vertices[11] = { rightTop.x, rightTop.y + offset };

                vertices[12] = { rightBottom.x, rightBottom.y - offset };
                vertices[13] = rightBottom;
                vertices[14] = rightBottom;
                vertices[15] = { rightBottom.x - offset, rightBottom.y };

                glUnmapBuffer(GL_ARRAY_BUFFER);
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}


void HeadUpDisplay::cancelSelection() noexcept
{
    m_selectionFrame.lastSelectedEntity = entt::null;
}


void HeadUpDisplay::resize(int width, int height) noexcept
{
    m_menu.resize(width, height);
}


bool HeadUpDisplay::isEntitySelected() const noexcept
{
    return (m_selectionFrame.lastSelectedEntity != entt::null);
}


entt::entity HeadUpDisplay::getLastSelectedEntity() const noexcept
{
    return m_selectionFrame.lastSelectedEntity;
}


uint32_t HeadUpDisplay::getTexture() const noexcept
{
    return m_previewTexture;
}