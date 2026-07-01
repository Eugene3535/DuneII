#include <cstring>
#include <cassert>
#include <algorithm>

#include <glad/glad.h>

#include "files/FileProvider.hpp"
#include "graphics/texture/Texture2D.hpp"
#include "graphics/vao/VertexBufferLayout.hpp"
#include "graphics/geometry/GeometryGenerator.hpp"
#include "game/scenes/mission/tilemap/Tilemap.hpp"
#include "game/Engine.hpp"
#include "game/scenes/mission/HUD/construction/ConstructionMenu.hpp"


#define PREVIEW_ICON_COLUMNS 3
#define PREVIEW_ICON_ROWS 6

#define DEFAULT_MENU_WIDTH  920.f
#define DEFAULT_MENU_HEIGHT 800.f
#define MENU_SCALE_FACTOR 0.7f

#define MOVE_FRAME_DELAY_TIME 0.25f

namespace
{
    constexpr float background_color[]      = { 155.f / 255.f, 160.f / 255.f, 163.f / 255.f, 1.f };
    constexpr float outline_color[]         = { 170.f / 255.f, 199.f / 255.f, 207.f / 255.f, 1.f };
    constexpr float cell_background_color[] = { 116.f / 255.f, 120.f / 255.f, 121.f / 255.f, 1.f };
    constexpr float selection_frame_color[] = { 1.f, 0.f, 0.f, 1.f };
}


ConstructionMenu::ConstructionMenu(Engine* engine, Tilemap& tilemap) noexcept:
    m_engine(engine),
    m_tilemap(tilemap),
    m_transform(),
    m_isShown(false)
{

}


ConstructionMenu::~ConstructionMenu()
{
    glDeleteTextures(3, m_userElements.textures);

    glDeleteVertexArrays(1, &m_userElements.vertexArrayObject);
	glDeleteBuffers(1, &m_userElements.vertexBufferObject);
    glDeleteBuffers(1, &m_userElements.selectionFrame.vertexArrayObject);

    glDeleteVertexArrays(1, &m_previewCells.vertexArrayObject);
	glDeleteBuffers(1, &m_previewCells.vertexBufferObject);

    glDeleteVertexArrays(1, &m_frames.vertexArrayObject);
	glDeleteBuffers(1, &m_frames.vertexBufferObject);
}


void ConstructionMenu::init(uint32_t texture) noexcept
{
    memset(&m_frames, 0, sizeof(m_frames));
    memset(&m_previewCells, 0, sizeof(m_previewCells));
    memset(&m_userElements, 0, sizeof(m_userElements));

    m_frames.program = m_engine->getShaderProgram("color_outline");
    m_previewCells.program = m_engine->getShaderProgram("sprite");
    assert(m_frames.program && m_previewCells.program);

    m_previewCells.texture = texture;

    m_transform.setOrigin(DEFAULT_MENU_WIDTH * 0.5f, DEFAULT_MENU_HEIGHT * 0.5f);

    createFrames();
    createPreviews();
    createUserElements();
}


void ConstructionMenu::update(float dt) noexcept
{
    m_userElements.selectionFrame.moveFrameDelayTimer += dt;
}


void ConstructionMenu::showEntityMenu(EntityIcon mainIcon, std::span<EntityIcon> menuIcons) noexcept
{
    if(mainIcon >= EntityIcon::MAX)
        return;

    auto setup_tex_coords = [this](void* data, EntityIcon preview, uint32_t offset) -> void
    {
        const size_t index = static_cast<size_t>(preview) << 2;
        const vec2s* texCoords = &m_textureGrid[index];
        vec4s* vertices = static_cast<vec4s*>(data) + offset;

        vertices[0].z = texCoords[0].x;
        vertices[0].w = texCoords[0].y;

        vertices[1].z = texCoords[1].x;
        vertices[1].w = texCoords[1].y;

        vertices[2].z = texCoords[2].x;
        vertices[2].w = texCoords[2].y;

        vertices[3].z = texCoords[3].x;
        vertices[3].w = texCoords[3].y;
    };

    constexpr size_t previewCount = PREVIEW_ICON_COLUMNS * PREVIEW_ICON_ROWS;

    glBindBuffer(GL_ARRAY_BUFFER, m_previewCells.vertexBufferObject);

    if(void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))
    {
        // Main preview
        setup_tex_coords(data, mainIcon, (previewCount << 2));
        m_userElements.lastSelectedPreview = mainIcon;

        // Others previews (if exists)
        if(!menuIcons.empty())
        {
            for (size_t i = 0; i < previewCount; ++i)
            {
                if(i < menuIcons.size())
                {
                    setup_tex_coords(data, menuIcons[i], (i << 2));
                    m_previews[i] = menuIcons[i];
                }
                else
                {
                    setup_tex_coords(data, EntityIcon::Empty_Cell, (i << 2));
                    m_previews[i] = EntityIcon::Empty_Cell;
                }
            }
        }
        else
        {
            for (size_t i = 0; i < previewCount; ++i)
            {
               setup_tex_coords(data, EntityIcon::Empty_Cell, (i << 2));
               m_previews[i] = EntityIcon::Empty_Cell;
            }
        }

        m_isShown = (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

//  Reset selection frame to start position
    m_userElements.selectionFrame.row = 0;
    m_userElements.selectionFrame.column = 0;
    updateSelection('A', true);
}


void ConstructionMenu::updateSelection(char keyCode, bool isForced) noexcept
{
    if (m_userElements.selectionFrame.moveFrameDelayTimer < MOVE_FRAME_DELAY_TIME)
        return;

    m_userElements.selectionFrame.moveFrameDelayTimer = 0;

    const int32_t oldRow = m_userElements.selectionFrame.row;
    const int32_t oldColumn = m_userElements.selectionFrame.column;

    switch (keyCode)
    {
        case 'W': m_userElements.selectionFrame.row--;    break;
        case 'A': m_userElements.selectionFrame.column--; break;
        case 'S': m_userElements.selectionFrame.row++;    break;
        case 'D': m_userElements.selectionFrame.column++; break;

        default:
            break;
    }

    bool needUpdateOutline = // We check that the position of the frame has not changed, and that the frame is located within the grid.
        ((oldRow + oldColumn) != (m_userElements.selectionFrame.row + m_userElements.selectionFrame.column)) &&
        ((m_userElements.selectionFrame.row > -1) && (m_userElements.selectionFrame.row <= PREVIEW_ICON_ROWS)) &&
        ((m_userElements.selectionFrame.column > -1) && (m_userElements.selectionFrame.column < PREVIEW_ICON_COLUMNS));

//  Returning to the grid limits
    m_userElements.selectionFrame.row    = std::clamp(m_userElements.selectionFrame.row,    0, PREVIEW_ICON_ROWS);
    m_userElements.selectionFrame.column = std::clamp(m_userElements.selectionFrame.column, 0, PREVIEW_ICON_COLUMNS - 1);

    if ( (!needUpdateOutline) && (!isForced) )
        return;

    auto switch_button_outline = [this](void* data, uint32_t buttonIndex) -> void
    {
        constexpr vec4s startFrame = { 50.f, 30.f, 150.f, 50.f };
        const float thickness = 2;

        GeometryGenerator generator;

        auto outlineVertices = generator.createOutline(4, [startFrame, buttonIndex](size_t index) -> vec2s
        {
            const float offset = 160.f;

            switch (index)
            {
                case 0: return { startFrame.x + buttonIndex * offset,                startFrame.y                };
                case 1: return { startFrame.x + startFrame.z + buttonIndex * offset, startFrame.y                };
                case 2: return { startFrame.x + startFrame.z + buttonIndex * offset, startFrame.y + startFrame.w };
                case 3: return { startFrame.x + buttonIndex * offset,                startFrame.y + startFrame.w };

                default: return { 0.f, 0.f };
            }
        }, thickness);

        float* vertices = static_cast<float*>(data) + 48; // TODO: fix magic num
        memcpy(vertices, outlineVertices.data(), sizeof(float) * outlineVertices.size());
    };

    auto switch_preview_outline = [this](void* data) -> void
    {
        const int32_t column = m_userElements.selectionFrame.column;
        const int32_t row = m_userElements.selectionFrame.row - 1;

        constexpr vec2s startPos = { 50.f, 100.f  };
        constexpr vec2s cellSize = { 150.f, 100.f };

        const float indent = 10.f;
        const float thickness = 4;

        const float left   = startPos.x + cellSize.x * column + indent * column;
        const float top    = startPos.y + cellSize.y * row + indent * row;
        const float right  = left + cellSize.x;
        const float bottom = top + cellSize.y;

        const std::array<vec2s, 4> newFrameVertices =
        {
            left,  top,
            right, top,
            right, bottom,
            left,  bottom
        };

        GeometryGenerator generator;

        auto outlineVertices = generator.createOutline(4, [&newFrameVertices](size_t index) -> vec2s
        {
            const float offset = 160.f;

            switch (index)
            {
                case 0: return newFrameVertices[0];
                case 1: return newFrameVertices[1];
                case 2: return newFrameVertices[2];
                case 3: return newFrameVertices[3];

                default: return { 0.f, 0.f };
            }
        }, thickness);

        float* vertices = static_cast<float*>(data) + 48; // TODO: fix magic num
        memcpy(vertices, outlineVertices.data(), sizeof(float) * outlineVertices.size());
    };

    auto setup_main_icon = [this](void* data, EntityIcon preview) -> void
    {
        constexpr size_t offset = (PREVIEW_ICON_COLUMNS * PREVIEW_ICON_ROWS) << 2;
        const size_t index = static_cast<size_t>(preview) << 2;
        const vec2s* texCoords = &m_textureGrid[index];
        vec4s* vertices = static_cast<vec4s*>(data) + offset;

        vertices[0].z = texCoords[0].x;
        vertices[0].w = texCoords[0].y;

        vertices[1].z = texCoords[1].x;
        vertices[1].w = texCoords[1].y;

        vertices[2].z = texCoords[2].x;
        vertices[2].w = texCoords[2].y;

        vertices[3].z = texCoords[3].x;
        vertices[3].w = texCoords[3].y;
    };

//  Update outline
    glBindBuffer(GL_ARRAY_BUFFER, m_userElements.vertexBufferObject);

    if(void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))
    {
        if (m_userElements.selectionFrame.row)
        {
            switch_preview_outline(data);
        }
        else
        {
            switch_button_outline(data, m_userElements.selectionFrame.column);
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

//  Update main preview
    glBindBuffer(GL_ARRAY_BUFFER, m_previewCells.vertexBufferObject);

    if(void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))
    {
        if (m_userElements.selectionFrame.row)
        {    
            const auto selectedPreview = getSelectedPreview();

            if (selectedPreview == EntityIcon::Empty_Cell)
            {
                setup_main_icon(data, m_userElements.lastSelectedPreview);
            }
            else if (selectedPreview != EntityIcon::INVALID)
            {
                setup_main_icon(data, selectedPreview);
            }
        }
        else
        {
            setup_main_icon(data, m_userElements.lastSelectedPreview);
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void ConstructionMenu::hide() noexcept
{
    m_isShown = false;
}


void ConstructionMenu::draw(const mat4s& projection) const noexcept
{
    if (!m_isShown)
        return;
        
    mat4s currentWorldMatrix = projection;
    mat4s modelView = getTransform().getMatrix();
    mat4s result;
    glm_mul(currentWorldMatrix.raw, modelView.raw, result.raw);
    m_engine->updateUniformBuffer(result);

    drawFrames();
    drawPreviews();
    drawUserElements();
}


void ConstructionMenu::resize(int width, int height) noexcept
{
//  Main menu
    float dx = width * MENU_SCALE_FACTOR / DEFAULT_MENU_WIDTH;
    float dy = height * MENU_SCALE_FACTOR / DEFAULT_MENU_HEIGHT;
    m_transform.setScale(dx, dy);
    m_transform.setPosition(width * 0.5f, height * 0.5f);
}


EntityIcon ConstructionMenu::getSelectedPreview() const noexcept
{
    if (m_isShown)
    {
        const int32_t row = m_userElements.selectionFrame.row;
        const int32_t column = m_userElements.selectionFrame.column;

        if (row)
        {
            if (row == 1)
                return m_previews[column];

            return m_previews[(row - 1) * PREVIEW_ICON_COLUMNS + column];
        }
    }

    return EntityIcon::INVALID;
}


ConstructionMenu::ButtonType ConstructionMenu::getSelectedButton() const noexcept
{
    const int32_t row = m_userElements.selectionFrame.row;
    const int32_t column = m_userElements.selectionFrame.column;
    
    if(row == 0) // first line
        return static_cast<ButtonType>(column);

    return ButtonType::NotSelected;
}


bool ConstructionMenu::isShown() const noexcept
{
    return m_isShown;
}


const Transform2D& ConstructionMenu::getTransform() const noexcept
{
    return m_transform;
}


void ConstructionMenu::createFrames() noexcept
{
    if(const GLint uniformColor = glGetUniformLocation(m_frames.program, "outlineColor"); uniformColor != -1)
        m_frames.uniformColor = uniformColor;

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
    createRectangle({0.f, 0.f, DEFAULT_MENU_WIDTH, DEFAULT_MENU_HEIGHT}, m_frames.rootWidget.background, m_frames.rootWidget.outline);

//  Entity presentation widget
    createRectangle({580.f, 100.f, 300.f, 200.f}, m_frames.entityWidget.background, m_frames.entityWidget.outline);

//  Entity widget label
    createRectangle({580.f, 315.f, 300.f, 50.f}, m_frames.entityWidgetLabel.background, m_frames.entityWidgetLabel.outline);

//  Entity property labels
    createRectangle({580.f, 450.f, 300.f, 50.f}, m_frames.entityWidgetParams[0].background, m_frames.entityWidgetParams[0].outline);
    createRectangle({580.f, 550.f, 300.f, 50.f}, m_frames.entityWidgetParams[1].background, m_frames.entityWidgetParams[1].outline);
    createRectangle({580.f, 650.f, 300.f, 50.f}, m_frames.entityWidgetParams[2].background, m_frames.entityWidgetParams[2].outline);

//  Unload to GPU
    glCreateBuffers(1, &m_frames.vertexBufferObject);
    glNamedBufferData(m_frames.vertexBufferObject, buffer.size() * sizeof(float), buffer.data(), GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &m_frames.vertexArrayObject);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float2 };
    VertexBufferLayout layout(attributes);
    layout.createVertexInputState(m_frames.vertexArrayObject, m_frames.vertexBufferObject);
}


void ConstructionMenu::createPreviews() noexcept
{
    int32_t texWidth, texHeight;
    glGetTextureLevelParameteriv(m_previewCells.texture, 0, GL_TEXTURE_WIDTH, &texWidth);
	glGetTextureLevelParameteriv(m_previewCells.texture, 0, GL_TEXTURE_HEIGHT, &texHeight);

    const int32_t columns      = 6; // The number of tiles in the texture horizontally
    const int32_t rows         = 7;    // and vertically
    const int32_t spriteWidth  = texWidth / columns;
    const int32_t spriteHeight = texHeight / rows;
    const vec2s   ratio        = { 1.f / texWidth, 1.f / texHeight };

    m_textureGrid.reserve((rows * columns) << 2);
    m_previews.resize(PREVIEW_ICON_COLUMNS * PREVIEW_ICON_ROWS, EntityIcon::Empty_Cell);

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
    vertices.reserve((PREVIEW_ICON_COLUMNS * PREVIEW_ICON_ROWS + 1) << 2); // +1 for major preview in the right-top corner

    const vec2s cellSize = { 150.f, 100.f };
    const vec2s startPos = { 50.f, 100.f  };
    const float indent = 10.f;

    const size_t index = static_cast<size_t>(EntityIcon::Empty_Cell) << 2;
    const vec2s* texCoords = &m_textureGrid[index]; // offset to gray color cell (empty preview)

    for (int32_t y = 0; y < PREVIEW_ICON_ROWS; ++y)
    {
        for (int32_t x = 0; x < PREVIEW_ICON_COLUMNS; ++x)
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
    vertices.push_back({ 590.f, 120.f,  texCoords[0].x, texCoords[0].y });
    vertices.push_back({ 870.f, 120.f,  texCoords[1].x, texCoords[1].y });
    vertices.push_back({ 870.f, 280.f,  texCoords[2].x, texCoords[2].y });
    vertices.push_back({ 590.f, 280.f,  texCoords[3].x, texCoords[3].y });

    m_previewCells.cellCount = static_cast<uint32_t>(vertices.size() >> 2);
    m_previewCells.cellWidth = spriteWidth;
    m_previewCells.cellHeight = spriteHeight;

//  Side bar entity preview
    vertices.push_back({ 950.f,  0.f,   texCoords[0].x, texCoords[0].y });
    vertices.push_back({ 1100.f, 0.f,   texCoords[1].x, texCoords[1].y });
    vertices.push_back({ 1100.f, 100.f, texCoords[2].x, texCoords[2].y });
    vertices.push_back({ 950.f,  100.f, texCoords[3].x, texCoords[3].y });

    glCreateBuffers(1, &m_previewCells.vertexBufferObject);
    glNamedBufferData(m_previewCells.vertexBufferObject, vertices.size() * sizeof(vec4s), vertices.data(), GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &m_previewCells.vertexArrayObject);
    const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float4 };
    VertexBufferLayout layout(attributes);
    layout.createVertexInputState(m_previewCells.vertexArrayObject, m_previewCells.vertexBufferObject);
}


void ConstructionMenu::createUserElements() noexcept
{
    glCreateTextures(GL_TEXTURE_2D, 3, m_userElements.textures);

    uint32_t currentTexture = 0;

    for (const auto textureName : { BUTTON_EXIT_RU_PNG, BUTTON_REPAIR_RU_PNG, BUTTON_STOP_RU_PNG })
    {
        Texture2D texture = {.handle = m_userElements.textures[currentTexture++] };

        if(!texture.loadFromFile(FileProvider::findPathToFile(textureName)))
            return;
    }

    std::vector<float> vertices;
    constexpr vec4s posFrame = { 50.f, 30.f, 150.f, 50.f };
    const float offset = 160.f;

    for (size_t i = 0; i < 3; ++i)
    {
        GLint width, height;
        glGetTextureLevelParameteriv(m_userElements.textures[i], 0, GL_TEXTURE_WIDTH, &width);
        glGetTextureLevelParameteriv(m_userElements.textures[i], 0, GL_TEXTURE_HEIGHT, &height);

        std::array<float, 16> quad;

//  Positions
        quad[0] = posFrame.x + i * offset;
        quad[1] = posFrame.y;

        quad[4] = posFrame.x + posFrame.z + i * offset;
        quad[5] = posFrame.y;

        quad[8] = posFrame.x + posFrame.z + i * offset;
        quad[9] = posFrame.y + posFrame.w;

        quad[12] = posFrame.x + i * offset;
        quad[13] = posFrame.y + posFrame.w;

//  Texture coords
        const ivec4s texFrame = { 0, 0, width, height };
        const vec2s ratio = { 1.f / static_cast<float>(width), 1.f / static_cast<float>(height) };

        const float left   = texFrame.x * ratio.x;
        const float top    = texFrame.y * ratio.y;
        const float right  = (texFrame.x + texFrame.z) * ratio.x;
        const float bottom = (texFrame.y + texFrame.w) * ratio.y;

        quad[2] = left;
        quad[3] = top;

        quad[6] = right;
        quad[7] = top;

        quad[10] = right;
        quad[11] = bottom;

        quad[14] = left;
        quad[15] = bottom;

//  Assign sprite
        Sprite2D* button = &m_userElements.buttonExit;
        if(i == 1) button = &m_userElements.buttonRepair;
        if(i == 2) button = &m_userElements.buttonStop;

        button->texture = m_userElements.textures[i];
        button->frame   = (i << 2);
        button->width   = width;
        button->height  = height;

        vertices.insert(vertices.end(), quad.begin(), quad.end());
    }

    {// Outline
        GeometryGenerator generator;
        const float thickness = 2;

        auto outlineVertices = generator.createOutline(4, [posFrame](size_t index) -> vec2s
        {
            switch (index)
            {
                case 0: return { static_cast<float>(posFrame.x),              static_cast<float>(posFrame.y)              };
                case 1: return { static_cast<float>(posFrame.x + posFrame.z), static_cast<float>(posFrame.y)              };
                case 2: return { static_cast<float>(posFrame.x + posFrame.z), static_cast<float>(posFrame.y + posFrame.w) };
                case 3: return { static_cast<float>(posFrame.x),              static_cast<float>(posFrame.y + posFrame.w) };

                default: return { 0.f, 0.f };
            }
        }, thickness);

        vertices.insert(vertices.end(), outlineVertices.begin(), outlineVertices.end());
        m_userElements.selectionFrame.count = (outlineVertices.size() >> 1);
    }

//  Unload to GPU
    glCreateBuffers(1, &m_userElements.vertexBufferObject);
    glNamedBufferData(m_userElements.vertexBufferObject, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    {// Vertex array object for buttons
        glGenVertexArrays(1, &m_userElements.vertexArrayObject);
        const std::array<VertexBufferLayout::Attribute, 1> attributes = { VertexBufferLayout::Attribute::Float4 };
        VertexBufferLayout layout(attributes);
        layout.createVertexInputState(m_userElements.vertexArrayObject, m_userElements.vertexBufferObject);
    }

    {// Vertex array object for selection frame
        glGenVertexArrays(1, &m_userElements.selectionFrame.vertexArrayObject);
        const std::array<VertexBufferLayout::Attribute, 1> attributes = { VertexBufferLayout::Attribute::Float2 };
        VertexBufferLayout layout(attributes);
        layout.createVertexInputState(m_userElements.selectionFrame.vertexArrayObject, m_userElements.vertexBufferObject);
    }

    m_userElements.program = m_previewCells.program;
    m_userElements.selectionFrame.program = m_frames.program;
}


void ConstructionMenu::drawFrames() const noexcept
{
    glUseProgram(m_frames.program);
    uint32_t startFrame = 0;

    glBindVertexArray(m_frames.vertexArrayObject);

    glUniform4fv(m_frames.uniformColor, 1, background_color);
    glDrawArrays(GL_TRIANGLE_FAN, 0, m_frames.rootWidget.background);
    glUniform4fv(m_frames.uniformColor, 1, outline_color);
    glDrawArrays(GL_TRIANGLE_STRIP, m_frames.rootWidget.background, m_frames.rootWidget.outline);
    startFrame = m_frames.rootWidget.background + m_frames.rootWidget.outline;

    glUniform4fv(m_frames.uniformColor, 1, cell_background_color);
    glDrawArrays(GL_TRIANGLE_FAN, startFrame, m_frames.entityWidget.background);
    glUniform4fv(m_frames.uniformColor, 1, outline_color);
    glDrawArrays(GL_TRIANGLE_STRIP, startFrame + m_frames.entityWidget.background, m_frames.entityWidget.outline);
    startFrame += m_frames.entityWidget.background + m_frames.entityWidget.outline;

    glUniform4fv(m_frames.uniformColor, 1, cell_background_color);
    glDrawArrays(GL_TRIANGLE_FAN, startFrame, m_frames.entityWidgetLabel.background);
    glUniform4fv(m_frames.uniformColor, 1, outline_color);
    glDrawArrays(GL_TRIANGLE_STRIP, startFrame + m_frames.entityWidgetLabel.background, m_frames.entityWidgetLabel.outline);
    startFrame += m_frames.entityWidgetLabel.background + m_frames.entityWidgetLabel.outline;

    for (uint32_t i = 0; i < 3; ++i)
    {
        glUniform4fv(m_frames.uniformColor, 1, cell_background_color);
        glDrawArrays(GL_TRIANGLE_FAN, startFrame, m_frames.entityWidgetParams[i].background);
        glUniform4fv(m_frames.uniformColor, 1, outline_color);
        glDrawArrays(GL_TRIANGLE_STRIP, startFrame + m_frames.entityWidgetParams[i].background, m_frames.entityWidgetParams[i].outline);
        startFrame += m_frames.entityWidgetParams[i].background + m_frames.entityWidgetParams[i].outline;
    }
}


void ConstructionMenu::drawPreviews() const noexcept
{
    glUseProgram(m_previewCells.program);
    glBindTextureUnit(0, m_previewCells.texture);
    glBindVertexArray(m_previewCells.vertexArrayObject);

    for (uint32_t i = 0; i < m_previewCells.cellCount; ++i)
        glDrawArrays(GL_TRIANGLE_FAN, i << 2, 4);

    glBindTextureUnit(0, 0);
}


void ConstructionMenu::drawUserElements() const noexcept
{
//  Draw buttons
    glBindVertexArray(m_userElements.vertexArrayObject);

    glBindTextureUnit(0, m_userElements.buttonExit.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_userElements.buttonExit.frame, 4);

    glBindTextureUnit(0, m_userElements.buttonRepair.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_userElements.buttonRepair.frame, 4);

    glBindTextureUnit(0, m_userElements.buttonStop.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_userElements.buttonStop.frame, 4);

    glBindTextureUnit(0, 0);

//  Draw selection frame
    glUseProgram(m_userElements.selectionFrame.program);
    glUniform4fv(m_frames.uniformColor, 1, selection_frame_color);

    glBindVertexArray(m_userElements.selectionFrame.vertexArrayObject);
    glDrawArrays(GL_TRIANGLE_STRIP, 24, m_userElements.selectionFrame.count); // Recalculating the offset: 2 float per vertex in outline VAO !!!
    glBindVertexArray(0);
}
