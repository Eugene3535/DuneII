#include <cmath>

#include <glad/glad.h>

#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "graphics/geometry/GeometryGenerator.hpp"
#include "game/scenes/mission/menu/ConstructionMenu.hpp"


static constexpr float background_color[]      = { 155.f / 255.f, 160.f / 255.f, 163.f / 255.f, 1.f };
static constexpr float outline_color[]         = { 170.f / 255.f, 199.f / 255.f, 207.f / 255.f, 1.f };
static constexpr float cell_background_color[] = { 116.f / 255.f, 120.f / 255.f, 121.f / 255.f, 1.f };


ConstructionMenu::ConstructionMenu(const ivec2s& windowSize) noexcept:
    m_windowSize(windowSize),
    m_transform(),
    m_vao(0),
    m_vbo(0),
    m_program(0),
    m_uniform(0),
    m_isEnabled(false)
{

}


ConstructionMenu::~ConstructionMenu()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}


void ConstructionMenu::init(uint32_t program) noexcept
{
    if(m_vbo && m_vao)
        return;

    m_program = program;

    if(GLint uniformColor = glGetUniformLocation(m_program, "outlineColor"); uniformColor != -1)
        m_uniform = uniformColor;

    GeometryGenerator generator;
    std::vector<float> buffer;
    const float thickness = 5.f;
    const float radius = 10.f;

    auto createRectangle = [&buffer, thickness, radius](vec4s bounds, uint32_t& background, uint32_t& outline) -> void
    {
        GeometryGenerator generator;
        auto rectVertices = generator.createRectWithRoundedEdges(bounds.x, bounds.y, bounds.z, bounds.w, radius);
        background = (rectVertices.size() >> 1);

        auto outlineVertices = generator.createOutline(background, [&rectVertices](size_t index) -> vec2s
        {
            const float* data = rectVertices.data() + (index << 1);

            return { data[0], data[1] };
        }, thickness);
        outline = (outlineVertices.size() >> 1);

        buffer.insert(buffer.end(), rectVertices.begin(), rectVertices.end());
        buffer.insert(buffer.end(), outlineVertices.begin(), outlineVertices.end());
    };

//  Background
    createRectangle({0.f, 0.f, 920.f, 800.f}, m_rootWindow.background, m_rootWindow.outline);

//  Entity presentation window
    createRectangle({580.f, 50.f, 300.f, 200.f}, m_entityWindow.background, m_entityWindow.outline);

//  Entity window label
    createRectangle({580.f, 265.f, 300.f, 50.f}, m_entityWindowLabel.background, m_entityWindowLabel.outline);

//  Entity properties label
    createRectangle({580.f, 400.f, 300.f, 50.f}, m_entityWindowParams[0].background, m_entityWindowParams[0].outline);
    createRectangle({580.f, 500.f, 300.f, 50.f}, m_entityWindowParams[1].background, m_entityWindowParams[1].outline);
    createRectangle({580.f, 600.f, 300.f, 50.f}, m_entityWindowParams[2].background, m_entityWindowParams[2].outline);

//  Unload to GPU
    glCreateBuffers(1, &m_vbo);
    glNamedBufferData(m_vbo, buffer.size() * sizeof(float), buffer.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vao);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
    VertexArrayObject::createVertexInputState(m_vao, m_vbo, attributes);
}


void ConstructionMenu::update() noexcept
{
    if(m_isEnabled)
    {
        const vec2s center = { m_windowSize.x * 0.5f, m_windowSize.y * 0.5f };

        m_transform.setOrigin(460, 400);
        m_transform.setPosition(center);
    }
}


void ConstructionMenu::enable() noexcept
{
    m_isEnabled = true;
}


void ConstructionMenu::disable() noexcept
{
    m_isEnabled = false;
}


void ConstructionMenu::draw() noexcept
{
    glUseProgram(m_program);
    uint32_t startFrame = 0;

    glBindVertexArray(m_vao);

    glUniform4fv(m_uniform, 1, background_color);
    glDrawArrays(GL_TRIANGLE_FAN, 0, m_rootWindow.background);
    glUniform4fv(m_uniform, 1, outline_color);
    glDrawArrays(GL_TRIANGLE_STRIP, m_rootWindow.background, m_rootWindow.outline);
    startFrame = m_rootWindow.background + m_rootWindow.outline;

    glUniform4fv(m_uniform, 1, cell_background_color);
    glDrawArrays(GL_TRIANGLE_FAN, startFrame, m_entityWindow.background);
    glUniform4fv(m_uniform, 1, outline_color);
    glDrawArrays(GL_TRIANGLE_STRIP, startFrame + m_entityWindow.background, m_entityWindow.outline);
    startFrame += m_entityWindow.background + m_entityWindow.outline;

    glUniform4fv(m_uniform, 1, cell_background_color);
    glDrawArrays(GL_TRIANGLE_FAN, startFrame, m_entityWindowLabel.background);
    glUniform4fv(m_uniform, 1, outline_color);
    glDrawArrays(GL_TRIANGLE_STRIP, startFrame + m_entityWindowLabel.background, m_entityWindowLabel.outline);
    startFrame += m_entityWindowLabel.background + m_entityWindowLabel.outline;

    for (uint32_t i = 0; i < 3; ++i)
    {
        glUniform4fv(m_uniform, 1, cell_background_color);
        glDrawArrays(GL_TRIANGLE_FAN, startFrame, m_entityWindowParams[i].background);
        glUniform4fv(m_uniform, 1, outline_color);
        glDrawArrays(GL_TRIANGLE_STRIP, startFrame + m_entityWindowParams[i].background, m_entityWindowParams[i].outline);
        startFrame += m_entityWindowParams[i].background + m_entityWindowParams[i].outline;
    }
    
    glBindVertexArray(0);
}


bool ConstructionMenu::isEnabled() const noexcept
{
    return m_isEnabled;
}


const Transform2D& ConstructionMenu::getTransform() const noexcept
{
    return m_transform;
}