#include "resources/files/FileProvider.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "game/scenes/mission/builder/Builder.hpp"
#include "game/scenes/mission/HUD/HeadUpDisplay.hpp"


HeadUpDisplay::HeadUpDisplay(Builder& builder) noexcept:
    m_builder(builder),
    m_currentCursor(nullptr)
{
    m_selectionFrame.vbo = 0;
    m_selectionFrame.vao = 0;
    m_selectionFrame.timer = 0.f;
    m_selectionFrame.enabled = false;
}


HeadUpDisplay::~HeadUpDisplay()
{
    glDeleteVertexArrays(1, &m_selectionFrame.vao);
    glDeleteBuffers(1, &m_selectionFrame.vbo);
}


void HeadUpDisplay::init(std::span<const Sprite> crosshairs) noexcept
{
//  Cursors
    m_releasedCursor = crosshairs[0];
    m_capturedCursor = crosshairs[1];
    m_cursorTransform.setOrigin(m_releasedCursor.width * 0.5f, m_releasedCursor.height * 0.5f);
    m_cursorTransform.setScale(0.5f, 0.5f);
    m_currentCursor = &m_releasedCursor;

//  Selection frame
    const float vertexData[32] = {}; // 16 vertices
    glCreateBuffers(1, &m_selectionFrame.vbo);
	glNamedBufferData(m_selectionFrame.vbo, sizeof(vertexData), vertexData, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &m_selectionFrame.vao);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
	VertexArrayObject::createVertexInputState(m_selectionFrame.vao, m_selectionFrame.vbo, attributes);
}


void HeadUpDisplay::update(const vec2s position, float dt, bool isClicked) noexcept
{
    m_selectionFrame.timer += dt;

    if(m_selectionFrame.timer > 0.25f)
        m_selectionFrame.timer = 0.f;

    m_cursorTransform.setPosition(position);

    if(isClicked)
    {
        if(auto entity = m_builder.getEntityUnderCursor(position); entity.has_value())
        {
            auto& registry = m_builder.getRegistry();

            if(auto* building = registry.try_get<Structure>(entity.value()))
            {
                bool isSelectable = ((building->type != Structure::Type::SLAB_1x1) &&
                                     (building->type != Structure::Type::SLAB_2x2) && 
                                     (building->type != Structure::Type::WALL)     && 
                                      building->type <  Structure::Type::MAX);

                if(isSelectable)
                {
                    const auto bounds = registry.get<ivec4s>(entity.value());
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
    }
}


void HeadUpDisplay::drawSelection() const noexcept
{
    if(m_selectionFrame.enabled && m_selectionFrame.timer < 0.125f)
    {
        glBindVertexArray(m_selectionFrame.vao);
        glDrawArrays(GL_LINES, 0, 16);
        glBindVertexArray(0);
    }
}


void HeadUpDisplay::drawCursor() const noexcept
{
    if(m_currentCursor)
    {
        glBindTextureUnit(0, m_currentCursor->texture);
        glDrawArrays(GL_TRIANGLE_FAN, m_currentCursor->frame, 4);
        glBindTextureUnit(0, 0);
    }
}


const Transform2D& HeadUpDisplay::getCursorTransform() const noexcept
{
    return m_cursorTransform;
}