#include <algorithm>

#include "resources/files/FileProvider.hpp"
#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "game/scenes/mission/builder/Builder.hpp"
#include "game/Engine.hpp"
#include "game/scenes/mission/HUD/HeadUpDisplay.hpp"

#define BLINK_PERIOD 0.125f
#define BLINK_LOOP_TIME 0.25f


HeadUpDisplay::HeadUpDisplay(Engine* engine, const Transform2D& sceneTransform, const Builder& builder) noexcept:
    m_engine(engine),
    m_sceneTransform(sceneTransform),
    m_builder(builder),
    m_menu(engine),
    m_cursorTexture(0),
    m_program(0),
    m_clickTimer(0.f)
{
    m_selectionFrame.vbo = 0;
    m_selectionFrame.vao = 0;
    m_selectionFrame.blinkTimer = 0.f;
    m_selectionFrame.enabled = false;
    m_selectionFrame.lastSelectedEntity = entt::null;
}


HeadUpDisplay::~HeadUpDisplay()
{
    glDeleteTextures(1, &m_cursorTexture);
    glDeleteVertexArrays(1, &m_selectionFrame.vao);
    glDeleteBuffers(1, &m_selectionFrame.vbo);
}


bool HeadUpDisplay::init() noexcept
{
    m_program = m_engine->getShaderProgram("selection");
    
    if(!m_program)
        return false;

    glGenTextures(1, &m_cursorTexture);
    Texture crosshairTexture = {.handle = m_cursorTexture };

    if(!crosshairTexture.loadFromFile(FileProvider::findPathToFile(CROSSHAIRS_TILESHEET_PNG)))
        return false;

    m_sprites.loadSpriteSheet(FileProvider::findPathToFile(CURSOR_FRAME_XML), crosshairTexture);
    auto crosshairReleased = m_sprites.getSprite("Released");
    auto crosshairCaptured = m_sprites.getSprite("Captured");

    if(! (crosshairReleased && crosshairCaptured) )
        return false;

//  Cursors
    m_releasedCursor = crosshairReleased.value();
    m_capturedCursor = crosshairCaptured.value();
    m_cursorTransform.setOrigin(m_releasedCursor.width * 0.5f, m_releasedCursor.height * 0.5f);
    m_cursorTransform.setScale(0.5f, 0.5f);
    m_currentCursor = m_releasedCursor;

//  Selection frame
    glCreateBuffers(1, &m_selectionFrame.vbo);
	glNamedBufferData(m_selectionFrame.vbo, sizeof(float) << 5, nullptr, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &m_selectionFrame.vao);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
	VertexArrayObject::createVertexInputState(m_selectionFrame.vao, m_selectionFrame.vbo, attributes);

    m_menu.init();

    return true;
}


void HeadUpDisplay::update(float dt) noexcept
{
    m_clickTimer += dt;
    m_selectionFrame.blinkTimer += dt;

    if(m_selectionFrame.blinkTimer > BLINK_LOOP_TIME)
        m_selectionFrame.blinkTimer = 0.f;

    m_cursorTransform.setPosition(m_engine->getCursorPosition());
}


void HeadUpDisplay::runSelection() noexcept
{
    if(m_menu.isShown())
        return;

    vec2s cursorPosition = m_engine->getCursorPosition();
    vec2s scenePosition  = glms_vec2_negate(m_sceneTransform.getPosition());
    vec2s worldCoords    = glms_vec2_add(scenePosition, cursorPosition);

    const auto entity = m_builder.getEntityUnderCursor(worldCoords);

    if(entity == entt::null)
    {
        m_selectionFrame.enabled = false;
        m_selectionFrame.lastSelectedEntity = entt::null;

        return;
    }

    if(m_selectionFrame.lastSelectedEntity != entity)
    {
        m_selectionFrame.lastSelectedEntity = entity;
    }           
    else
    {
        if(m_clickTimer > BLINK_LOOP_TIME)
            m_menu.show(/* for entity */);

        return;
    }

    m_clickTimer = 0;

    auto& registry = m_builder.getRegistry();

    if(StructureInfo* info = registry.try_get<StructureInfo>(entity))
    {
        bool isSelectable = ((info->type != StructureInfo::Type::SLAB_1x1) &&
                             (info->type != StructureInfo::Type::SLAB_2x2) && 
                             (info->type != StructureInfo::Type::WALL)     && 
                              info->type <  StructureInfo::Type::MAX);

        if(isSelectable)
        {
            const auto bounds = registry.get<ivec4s>(entity);
            glBindBuffer(GL_ARRAY_BUFFER, m_selectionFrame.vbo);

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

                if(glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE)
                    m_selectionFrame.enabled = true;
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}


void HeadUpDisplay::cancelSelection() noexcept
{
    m_selectionFrame.enabled = false;
    m_selectionFrame.lastSelectedEntity = entt::null;
}


void HeadUpDisplay::hideMenu() noexcept
{
    m_menu.hide();
}


void HeadUpDisplay::drawSelection() const noexcept
{
    if(m_selectionFrame.enabled && m_selectionFrame.blinkTimer < BLINK_PERIOD)
    {
        glUseProgram(m_program);
        glBindVertexArray(m_selectionFrame.vao);
        glDrawArrays(GL_LINES, 0, 16);
    }
}


void HeadUpDisplay::drawCursor() const noexcept
{
    m_sprites.bind(true);
    glBindTextureUnit(0, m_cursorTexture);
    glDrawArrays(GL_TRIANGLE_FAN, m_currentCursor.frame, 4);
    glBindTextureUnit(0, 0);
}


void HeadUpDisplay::drawMenu() const noexcept
{
    m_menu.draw();
}


void HeadUpDisplay::resize(int width, int height) noexcept
{
    m_menu.resize(width, height);
}


bool HeadUpDisplay::isSelectionEnabled() const noexcept
{
    return m_selectionFrame.enabled;
}


bool HeadUpDisplay::isMenuShown() const noexcept
{
    return m_menu.isShown();
}


const Transform2D& HeadUpDisplay::getCursorTransform() const noexcept
{
    return m_cursorTransform;
}


const Transform2D& HeadUpDisplay::getMenuTransform() const noexcept
{
    return m_menu.getTransform();
}