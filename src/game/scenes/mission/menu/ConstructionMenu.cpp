#include <cstring>
#include <cassert>

#include <glad/glad.h>

#include "resources/files/FileProvider.hpp"
#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "graphics/geometry/GeometryGenerator.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/mission/menu/ConstructionMenu.hpp"


#define PREVIEW_CELL_COLUMNS 3
#define PREVIEW_CELL_ROWS 5

#define DEFAULT_MENU_WIDTH  920.f
#define DEFAULT_MENU_HEIGHT 800.f
#define MENU_SCALE_FACTOR 0.9f

namespace
{
    constexpr float background_color[]      = { 155.f / 255.f, 160.f / 255.f, 163.f / 255.f, 1.f };
    constexpr float outline_color[]         = { 170.f / 255.f, 199.f / 255.f, 207.f / 255.f, 1.f };
    constexpr float cell_background_color[] = { 116.f / 255.f, 120.f / 255.f, 121.f / 255.f, 1.f };
}


ConstructionMenu::ConstructionMenu(const DuneII* game) noexcept:
    m_transform(),
    m_isShown(false)
{
    memset(&m_frame, 0, sizeof(m_frame));
    memset(&m_previews, 0, sizeof(m_previews));

    m_frame.program    = game->getShaderProgram("color_outline");
    m_previews.program = game->getShaderProgram("sprite");

    m_transform.setOrigin(DEFAULT_MENU_WIDTH * 0.5f, DEFAULT_MENU_HEIGHT * 0.5f);
}


ConstructionMenu::~ConstructionMenu()
{
    glDeleteTextures(1, &m_previews.texture);
    glDeleteVertexArrays(1, &m_previews.vao);
	glDeleteBuffers(1, &m_previews.vbo);
	
    glDeleteVertexArrays(1, &m_frame.vao);
	glDeleteBuffers(1, &m_frame.vbo);
}


void ConstructionMenu::init() noexcept
{
    assert(m_frame.program != 0);
    assert(m_previews.program != 0);

    createFrame();
    createPreviews();
}


void ConstructionMenu::update() noexcept
{

}


void ConstructionMenu::show() noexcept
{
    m_isShown = true;
}


void ConstructionMenu::hide() noexcept
{
    m_isShown = false;
}


void ConstructionMenu::draw() noexcept
{
    drawFrame();
    drawPreviews();
}


void ConstructionMenu::resize(int width, int height) noexcept
{
    float dx = width * MENU_SCALE_FACTOR / DEFAULT_MENU_WIDTH;
    float dy = height * MENU_SCALE_FACTOR / DEFAULT_MENU_HEIGHT;
    m_transform.setScale(dx, dy);
    m_transform.setPosition(width * 0.5f, height * 0.5f);
}


bool ConstructionMenu::isShown() const noexcept
{
    return m_isShown;
}


const Transform2D& ConstructionMenu::getTransform() const noexcept
{
    return m_transform;
}


void ConstructionMenu::createFrame() noexcept
{ 
    if(const GLint uniformColor = glGetUniformLocation(m_frame.program, "outlineColor"); uniformColor != -1)
        m_frame.uniform = uniformColor;

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
    createRectangle({0.f, 0.f, DEFAULT_MENU_WIDTH, DEFAULT_MENU_HEIGHT}, m_frame.rootWidget.background, m_frame.rootWidget.outline);

//  Entity presentation цidget
    createRectangle({580.f, 50.f, 300.f, 200.f}, m_frame.entityWidget.background, m_frame.entityWidget.outline);

//  Entity цidget label
    createRectangle({580.f, 265.f, 300.f, 50.f}, m_frame.entityWidgetLabel.background, m_frame.entityWidgetLabel.outline);

//  Entity property labels
    createRectangle({580.f, 400.f, 300.f, 50.f}, m_frame.entityWidgetParams[0].background, m_frame.entityWidgetParams[0].outline);
    createRectangle({580.f, 500.f, 300.f, 50.f}, m_frame.entityWidgetParams[1].background, m_frame.entityWidgetParams[1].outline);
    createRectangle({580.f, 600.f, 300.f, 50.f}, m_frame.entityWidgetParams[2].background, m_frame.entityWidgetParams[2].outline);

//  Unload to GPU
    glCreateBuffers(1, &m_frame.vbo);
    glNamedBufferData(m_frame.vbo, buffer.size() * sizeof(float), buffer.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_frame.vao);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
    VertexArrayObject::createVertexInputState(m_frame.vao, m_frame.vbo, attributes);
}


void ConstructionMenu::createPreviews() noexcept
{
    glGenTextures(1, &m_previews.texture);

    Texture previewsTexture = {.handle = m_previews.texture };

    if(!previewsTexture.loadFromFile(FileProvider::findPathToFile(PREVIEWS_PNG)))
        return;

    const int32_t columns = 6; // The number of tiles in the texture horizontally
    const int32_t rows = 7;    // and vertically
    const int32_t spriteWidth = previewsTexture.width / columns;
    const int32_t spriteHeight = previewsTexture.height / rows;
    const vec2s ratio = { 1.f / previewsTexture.width, 1.f / previewsTexture.height };

    m_textureGrid.reserve((PREVIEW_CELL_COLUMNS * PREVIEW_CELL_ROWS) << 2);

    for (int32_t y = 0; y < rows; ++y)
    {
        for (int32_t x = 0; x < columns; ++x)
        {
            const ivec4s frame = { x * spriteWidth, y * spriteHeight, spriteWidth, spriteHeight };

            const float left   = frame.x * ratio.x;
            const float top    = frame.y * ratio.y;
            const float right  = (frame.x + frame.z) * ratio.x;
            const float bottom = (frame.y + frame.w) * ratio.y;

            m_textureGrid.push_back({ left,  top    });
            m_textureGrid.push_back({ right, top    });
            m_textureGrid.push_back({ right, bottom });
            m_textureGrid.push_back({ left,  bottom });
        }
    }

    std::vector<vec4s> vertices;
    vertices.reserve((PREVIEW_CELL_COLUMNS * PREVIEW_CELL_ROWS + 1) << 2); // +1 for major preview in the right-top corner

    const vec2s cellSize = { 150.f, 100.f };
    const vec2s startPos = { 50.f, 50.f };
    const float indent = 10.f;
    const vec2s* texCoords = &m_textureGrid[Empty_Cell << 2]; // offset to gray color cell (empty preview)

    for (int32_t y = 0; y < PREVIEW_CELL_ROWS; ++y)
    {
        for (int32_t x = 0; x < PREVIEW_CELL_COLUMNS; ++x)
        {
            const float left   = startPos.x + cellSize.x * x + indent * x;
            const float top    = startPos.y + cellSize.y * y + indent * y;
            const float right  = left + cellSize.x;
            const float bottom = top + cellSize.y;

            vertices.push_back({ left,  top,    texCoords[0].x, texCoords[0].y });
            vertices.push_back({ right, top,    texCoords[1].x, texCoords[1].y });
            vertices.push_back({ right, bottom, texCoords[2].x, texCoords[2].y });
            vertices.push_back({ left,  bottom, texCoords[3].x, texCoords[3].y });
        }
    }
    
//  Main preview
    vertices.push_back({ 600.f, 80.f,  texCoords[0].x, texCoords[0].y});
    vertices.push_back({ 860.f, 80.f,  texCoords[1].x, texCoords[1].y });
    vertices.push_back({ 860.f, 220.f, texCoords[2].x, texCoords[2].y });
    vertices.push_back({ 600.f, 220.f, texCoords[3].x, texCoords[3].y });
    m_previews.cellCount = static_cast<uint32_t>(vertices.size() >> 2);

    glCreateBuffers(1, &m_previews.vbo);
    glNamedBufferData(m_previews.vbo, vertices.size() * sizeof(vec4s), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_previews.vao);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float4 };
    VertexArrayObject::createVertexInputState(m_previews.vao, m_previews.vbo, attributes);
}


void ConstructionMenu::drawFrame() noexcept
{
    glUseProgram(m_frame.program);
    uint32_t startFrame = 0;

    glBindVertexArray(m_frame.vao);

    glUniform4fv(m_frame.uniform, 1, background_color);
    glDrawArrays(GL_TRIANGLE_FAN, 0, m_frame.rootWidget.background);
    glUniform4fv(m_frame.uniform, 1, outline_color);
    glDrawArrays(GL_TRIANGLE_STRIP, m_frame.rootWidget.background, m_frame.rootWidget.outline);
    startFrame = m_frame.rootWidget.background + m_frame.rootWidget.outline;

    glUniform4fv(m_frame.uniform, 1, cell_background_color);
    glDrawArrays(GL_TRIANGLE_FAN, startFrame, m_frame.entityWidget.background);
    glUniform4fv(m_frame.uniform, 1, outline_color);
    glDrawArrays(GL_TRIANGLE_STRIP, startFrame + m_frame.entityWidget.background, m_frame.entityWidget.outline);
    startFrame += m_frame.entityWidget.background + m_frame.entityWidget.outline;

    glUniform4fv(m_frame.uniform, 1, cell_background_color);
    glDrawArrays(GL_TRIANGLE_FAN, startFrame, m_frame.entityWidgetLabel.background);
    glUniform4fv(m_frame.uniform, 1, outline_color);
    glDrawArrays(GL_TRIANGLE_STRIP, startFrame + m_frame.entityWidgetLabel.background, m_frame.entityWidgetLabel.outline);
    startFrame += m_frame.entityWidgetLabel.background + m_frame.entityWidgetLabel.outline;

    for (uint32_t i = 0; i < 3; ++i)
    {
        glUniform4fv(m_frame.uniform, 1, cell_background_color);
        glDrawArrays(GL_TRIANGLE_FAN, startFrame, m_frame.entityWidgetParams[i].background);
        glUniform4fv(m_frame.uniform, 1, outline_color);
        glDrawArrays(GL_TRIANGLE_STRIP, startFrame + m_frame.entityWidgetParams[i].background, m_frame.entityWidgetParams[i].outline);
        startFrame += m_frame.entityWidgetParams[i].background + m_frame.entityWidgetParams[i].outline;
    }
}


void ConstructionMenu::drawPreviews() noexcept
{
    glUseProgram(m_previews.program);
    glBindTextureUnit(0, m_previews.texture);
    glBindVertexArray(m_previews.vao);

    for (uint32_t i = 0; i < m_previews.cellCount; ++i)
        glDrawArrays(GL_TRIANGLE_FAN, i << 2, 4);
    
    glBindTextureUnit(0, 0);
}