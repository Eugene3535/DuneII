#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "cglm/struct/affine-mat.h"

#include "files/FileProvider.hpp"
#include "graphics/texture/Texture2D.hpp"
#include "graphics/vao/VertexBufferLayout.hpp"
#include "scenes/mission/level/Level.hpp"
#include "app/window/WindowData.hpp"
#include "app/game/Game.hpp"
#include "scenes/mission/ui/construction/ConstructionMenu.hpp"
#include "scenes/mission/ui/HeadUpDisplay.hpp"


// The flicker period of the selection frame
#define BLINK_PERIOD 0.125f
#define BLINK_LOOP_TIME 0.25f


HeadUpDisplay::HeadUpDisplay(Game* game, Level& level, ConstructionMenu& menu) noexcept:
    m_game(game),
    m_level(level),
    m_menu(menu),
    m_previewIcons(game),
    m_tilemapProgram(0),
    m_previewTexture(0),
    m_spriteVertexBufferObject(0),
    m_spriteVertexArrayObject(0)
{
    m_cursor.texture = 0;
    m_cursor.program = 0;
    m_cursor.timer = 0.f;

    m_selectionFrame.vertexBufferObject = 0;
    m_selectionFrame.vertexArrayObject = 0;
    m_selectionFrame.blinkTimer = 0.f;
    m_selectionFrame.lastSelectedEntity = entt::null;
}


HeadUpDisplay::~HeadUpDisplay()
{
    std::array<uint32_t, 2> vertexArrays = { m_selectionFrame.vertexArrayObject, m_spriteVertexArrayObject   };
    std::array<uint32_t, 2> buffers      = { m_selectionFrame.vertexBufferObject, m_spriteVertexBufferObject };
    std::array<uint32_t, 2> textures     = { m_cursor.texture, m_previewTexture                              };

    auto& glResources = m_game->glResources;
    glResources.destroyHandles(GlResourceManager::GLVertexArray, vertexArrays);
    glResources.destroyHandles(GlResourceManager::GLBuffer, buffers);
    glResources.destroyHandles(GlResourceManager::GLTexture2D, textures);
}


bool HeadUpDisplay::init() noexcept
{
    m_cursor.program = m_game->glResources.getProgram("selection");
    m_tilemapProgram = m_game->glResources.getProgram("sprite");

    if (!(m_cursor.program && m_tilemapProgram))
        return false;

    auto& glResources       = m_game->glResources;
    auto textureHandles     = glResources.getHandles(GlResourceManager::GLTexture2D, 2);
    auto bufferHandles      = glResources.getHandles(GlResourceManager::GLBuffer, 2);
    auto vertexArrayHandles = glResources.getHandles(GlResourceManager::GLVertexArray, 2);

    m_cursor.texture = textureHandles[0];
    Texture2D crosshairTexture(m_cursor.texture);

    if (!crosshairTexture.loadFromFile(FileProvider::findPathToFile(CROSSHAIRS_TILESHEET_PNG)))
        return false;

    m_sprites.loadSpriteSheet(FileProvider::findPathToFile(CURSOR_FRAME_XML), m_cursor.texture);
    m_spriteVertexBufferObject = bufferHandles[0];
    m_spriteVertexArrayObject = vertexArrayHandles[0];
    m_sprites.pushVerticesOnGPU(m_spriteVertexBufferObject);
    const std::array<VertexBufferLayout::Attribute, 1> spriteAttributes{ VertexBufferLayout::Attribute::Float4 };
	VertexBufferLayout spriteLayout(spriteAttributes);
	spriteLayout.createVertexInputState(m_spriteVertexBufferObject, m_spriteVertexBufferObject);

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
    m_selectionFrame.vertexBufferObject = bufferHandles[1];
	glNamedBufferData(m_selectionFrame.vertexBufferObject, sizeof(float) << 5, nullptr, GL_DYNAMIC_DRAW);

	m_selectionFrame.vertexArrayObject = vertexArrayHandles[1];
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
    VertexBufferLayout layout(attributes);
	layout.createVertexInputState(m_selectionFrame.vertexArrayObject, m_selectionFrame.vertexBufferObject);

//  Entity preview
    m_previewTexture = textureHandles[1];
    Texture2D previewTexture(m_previewTexture);

    if (!previewTexture.loadFromFile(FileProvider::findPathToFile(PREVIEWS_PNG)))
        return false;

    if (m_previewIcons.loadFromTexture(previewTexture))
    {
        const ivec2s position = { 950, 0 };
        const ivec2s size = { 150, 100 };
        m_previewIcons.createIcons(position, size);

        return true;
    }

    return false;
}


void HeadUpDisplay::update(float dt, vec2s cursor) noexcept
{
    m_cursor.timer += dt;
    m_selectionFrame.blinkTimer += dt;

    if(m_selectionFrame.blinkTimer > BLINK_LOOP_TIME)
        m_selectionFrame.blinkTimer = 0.f;

    m_cursor.position = cursor;
    m_cursor.transform.setPosition(cursor);
}


void HeadUpDisplay::draw(const mat4s& projection) const noexcept
{
    mat4s currentWorldMatrix = projection;
    mat4s modelView;
    mat4s result;

    auto view = m_game->windowData.view;

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
        view->updateUniformBuffer(result);
        m_previewIcons.draw();

        auto& registry = m_game->registry;

        if (auto* component = registry.try_get<ConstructionInfo>(m_selectionFrame.lastSelectedEntity))
        {
            if (component->isUnderConstruction)
            {
                m_previewIcons.draw(component->mainIcon, component->progress);
            }
        }
    }

    if (!m_menu.isShown())
    {
        glUseProgram(m_tilemapProgram);

        modelView = m_cursor.transform.getMatrix();
        result = glms_mul(currentWorldMatrix, modelView);
        view->updateUniformBuffer(result);

        glBindVertexArray(m_spriteVertexArrayObject);
        glBindTextureUnit(0, m_cursor.texture);
        glDrawArrays(GL_TRIANGLE_FAN, m_currentCursor.frame, 4);
        glBindTextureUnit(0, 0);
        glBindVertexArray(0);
    }
}


void HeadUpDisplay::runSelection() noexcept
{
    if(m_menu.isShown())
        return;

    vec2s cursorPosition = m_cursor.position;
    vec2s scenePosition  = glms_vec2_negate(m_level.getPosition());
    vec2s worldCoords    = glms_vec2_add(scenePosition, cursorPosition);

    const auto entity = m_level.getEntityUnderCursor(worldCoords);

    if(entity == entt::null)
    {
        cancelSelection();

        return;
    }

    auto convert_building_type_to_preview_icon = [](StructureInfo::Type type) -> EntityIcon
    {
        switch (type)
        {
            case StructureInfo::Type::Slab_2x2:         return EntityIcon::Slab_2x2;
            case StructureInfo::Type::Palace:           return EntityIcon::Palace;
            case StructureInfo::Type::Vehicle:          return EntityIcon::Light_Vehicle_Factory;
            case StructureInfo::Type::HighTech:         return EntityIcon::High_Tech;
            case StructureInfo::Type::ConstructionYard: return EntityIcon::Construction_Yard;
            case StructureInfo::Type::WindTrap:         return EntityIcon::Wind_Trap;
            case StructureInfo::Type::Barracks:         return EntityIcon::Barracks;
            case StructureInfo::Type::Starport:         return EntityIcon::Starport;
            case StructureInfo::Type::Refinery:         return EntityIcon::Refinery;
            case StructureInfo::Type::Repair:           return EntityIcon::Repair;
            case StructureInfo::Type::Turret:           return EntityIcon::Turret;
            case StructureInfo::Type::RocketTurret:     return EntityIcon::Rocket_Turret;
            case StructureInfo::Type::Silo:             return EntityIcon::Spice_Silo;
            case StructureInfo::Type::Outpost:          return EntityIcon::Outpost;

            default: return EntityIcon::Empty_Cell;
        }
    };

    auto& registry = m_game->registry;

    if (m_selectionFrame.lastSelectedEntity != entity)
    {
        m_selectionFrame.lastSelectedEntity = entity;
    }
    else
    {
        if (m_cursor.timer > BLINK_LOOP_TIME)
        {
            if (StructureInfo* info = registry.try_get<StructureInfo>(entity))
            {
                const auto mainPreviewIcon = convert_building_type_to_preview_icon(info->type);

                if (mainPreviewIcon != EntityIcon::Empty_Cell)
                {
                    std::span<EntityIcon> previews;

                    if (auto* construction = registry.try_get<ConstructionInfo>(entity))
                        previews = std::span<EntityIcon>(construction->previewIcons);
                    
                    m_menu.showEntityMenu(mainPreviewIcon, previews);
                }
            }
        }

        return;
    }

    m_cursor.timer = 0;

    if (auto* component = registry.try_get<StructureInfo>(entity))
    {
        bool isSelectable = ((component->type != StructureInfo::Type::Slab_1x1) &&
                             (component->type != StructureInfo::Type::Slab_2x2) &&
                             (component->type != StructureInfo::Type::Wall)     &&
                              component->type <  StructureInfo::Type::Max);

        if (isSelectable)
        {
            const auto entityIcon = convert_building_type_to_preview_icon(component->type);

            if (entityIcon != EntityIcon::Empty_Cell)
                m_previewIcons.setPreviewIcon(entityIcon);

            if (auto* construction = registry.try_get<ConstructionInfo>(entity))
                if (construction->isUnderConstruction)
                    m_previewIcons.setConstructionIcon(construction->mainIcon);
            
            const auto bounds = registry.get<ivec4s>(entity);
            glBindBuffer(GL_ARRAY_BUFFER, m_selectionFrame.vertexBufferObject);

            if (void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))
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


void HeadUpDisplay::forceUpdateConstructionIcon(EntityIcon icon) noexcept
{
    m_previewIcons.setConstructionIcon(icon);
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