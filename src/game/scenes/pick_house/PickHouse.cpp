#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "cglm/struct/affine-mat.h"

#include "resources/files/FileProvider.hpp"
#include "resources/files/Shader.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "resources/gl_interfaces/texture/Texture.hpp"
#include "graphics/geometry/GeometryGenerator.hpp"
#include "game/Engine.hpp"
#include "game/scenes/pick_house/PickHouse.hpp"


#define ATREIDES_OUTLINE_POSITION_X 32.f
#define ORDOS_OUTLINE_POSITION_X 120.f
#define HARKONNEN_OUTLINE_POSITION_X 208.f
#define OUTLINE_POSITION_Y 136.f

#define DEFAULT_OUTLINE_WIDTH 80.f
#define DEFAULT_OUTLINE_HEIGHT 16.f
#define SWITCH_HOUSE_OUTLINE_DELAY 0.3f


PickHouse::PickHouse(Engine* engine) noexcept:
    Scene(engine, Scene::PICK_HOUSE),
    m_vertexBufferObject(0),
    m_vertexArrayObjects{0, 0},
    m_selectedHouse(HouseType::ATREIDES),
    m_timer(0.f),
    m_outlineNeedUpdate(true)
{
    m_background.vao     = 0;
    m_background.program = 0;
    m_background.sprite.frame = 0;

    m_outline.vao     = 0;
    m_outline.program = 0;
    m_outline.count   = 0;
}


PickHouse::~PickHouse()
{
    glDeleteTextures(1, &m_background.sprite.texture);
    glDeleteVertexArrays(2, m_vertexArrayObjects);
    glDeleteBuffers(1, &m_vertexBufferObject);
}


bool PickHouse::load(std::string_view info) noexcept
{
    if(m_isLoaded)
        return true;

    glGenTextures(1, &m_background.sprite.texture);
    glGenVertexArrays(2, m_vertexArrayObjects);
    glCreateBuffers(1, &m_vertexBufferObject);

    m_background.vao = m_vertexArrayObjects[0];
    m_outline.vao = m_vertexArrayObjects[1];

//  Textures
    Texture housesTexture = {.handle = m_background.sprite.texture };

    if(!housesTexture.loadFromFile(FileProvider::findPathToFile(HOUSES_PNG)))
        return false;

    m_background.sprite.width  = housesTexture.width;
    m_background.sprite.height = housesTexture.height;

//  Shaders
    {
        if(m_background.program = m_engine->getShaderProgram("sprite"); m_background.program == 0)
            return false;

        if(m_outline.program = m_engine->getShaderProgram("color_outline"); m_outline.program == 0)
            return false;
    }

    if(GLint uniformColor = glGetUniformLocation(m_outline.program, "outlineColor"); uniformColor != -1)
    {
        const float outlineColor[] = { 1.f, 0.f, 0.f, 1.f };
        glUseProgram(m_outline.program);
        glUniform4fv(uniformColor, 1, outlineColor);
        glUseProgram(0); 
    }

    std::vector<float> vertices;

//  Background sprite
    {
        const vec2s ratio = { 1.f / housesTexture.width, 1.f / housesTexture.height };
        const ivec4s textureRect = { 0, 0, housesTexture.width, housesTexture.height };

        std::array<float, 16> quad = {};

        quad[4]  = static_cast<float>(textureRect.z);
        quad[8]  = static_cast<float>(textureRect.z);
        quad[9]  = static_cast<float>(textureRect.w);
        quad[13] = static_cast<float>(textureRect.w);

        float left   = textureRect.x * ratio.x;
        float top    = textureRect.y * ratio.y;
        float right  = (textureRect.x + textureRect.z) * ratio.x;
        float bottom = (textureRect.y + textureRect.w) * ratio.y;

        quad[2] = left;
        quad[3] = top;

        quad[6] = right;
        quad[7] = top;

        quad[10] = right;
        quad[11] = bottom;

        quad[14] = left;
        quad[15] = bottom;

        vertices.insert(vertices.end(), quad.begin(), quad.end());
    }

//  Outline
    {
        GeometryGenerator generator;
        const vec2s outlineSize = { DEFAULT_OUTLINE_WIDTH, DEFAULT_OUTLINE_HEIGHT };

        auto outlineVertices = generator.createOutline(4, [outlineSize](size_t index) -> vec2s
        {
            switch (index)
            {
                default:
                case 0: return { 0, 0 };
                case 1: return { outlineSize.x, 0 };
                case 2: return { outlineSize.x, outlineSize.y };
                case 3: return { 0, outlineSize.y };
            }
        });

        vertices.insert(vertices.end(), outlineVertices.begin(), outlineVertices.end());
        m_outline.count = (outlineVertices.size() >> 1);
    }

    glNamedBufferData(m_vertexBufferObject, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    const std::array<VertexBufferLayout::Attribute, 1> spriteAttributes{ VertexBufferLayout::Attribute::Float4 };
    VertexArrayObject::createVertexInputState(m_background.vao, m_vertexBufferObject, spriteAttributes);

    const std::array<VertexBufferLayout::Attribute, 1> outlineAttributes{ VertexBufferLayout::Attribute::Float2 };
    VertexArrayObject::createVertexInputState(m_outline.vao, m_vertexBufferObject, outlineAttributes);

    m_isLoaded = true;

    return m_isLoaded;
}


void PickHouse::update(float dt) noexcept
{
    if (!m_isLoaded)
        return;

    m_timer += dt;

    if (m_timer > SWITCH_HOUSE_OUTLINE_DELAY)
    {
        m_timer = 0;
        m_outlineNeedUpdate = true;

        switch (m_selectedHouse)
        {
            case HouseType::ATREIDES:
                if (m_engine->isKeyPressed(GLFW_KEY_RIGHT))
                    m_selectedHouse = HouseType::ORDOS;
                break;

            case HouseType::ORDOS:
                if (m_engine->isKeyPressed(GLFW_KEY_LEFT))
                    m_selectedHouse = HouseType::ATREIDES;

                if (m_engine->isKeyPressed(GLFW_KEY_RIGHT))
                    m_selectedHouse = HouseType::HARKONNEN;
                break;

            case HouseType::HARKONNEN:
                if (m_engine->isKeyPressed(GLFW_KEY_LEFT))
                    m_selectedHouse = HouseType::ORDOS;
                break;

            default:
                break;
        }
    }

    if (m_engine->isKeyPressed(GLFW_KEY_ENTER))
        m_engine->switchScene(this, Scene::MISSION);

    if(m_outlineNeedUpdate)
    {
        auto windowSize = m_engine->getWindowsSize();
        vec2 size = { static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) };

        float dx = size[0] / m_background.sprite.width;
        float dy = size[1] / m_background.sprite.height;
        float outlinePositionX = 0;

        switch (m_selectedHouse)
        {
            case HouseType::ATREIDES:
                outlinePositionX = ATREIDES_OUTLINE_POSITION_X * dx;
            break;

            case HouseType::ORDOS:
                outlinePositionX = ORDOS_OUTLINE_POSITION_X * dx;
            break;

            case HouseType::HARKONNEN:
                outlinePositionX = HARKONNEN_OUTLINE_POSITION_X * dx;
            break;
            
            default:
                break;
        }

        m_outline.transform.setPosition(outlinePositionX, OUTLINE_POSITION_Y * dy);
        m_outlineNeedUpdate = false;
    }
}


void PickHouse::draw() noexcept
{
    if(!m_isLoaded)
        return;

    auto& camera = m_engine->camera;
        
    alignas(16) mat4s MVP = camera.getModelViewProjectionMatrix();
    alignas(16) mat4s modelView;
    alignas(16) mat4s result;

//  Draw background
    modelView = m_background.transform.getMatrix();
    result = glms_mul(MVP, modelView);
    camera.updateUniformBuffer(result.raw);

    glUseProgram(m_background.program);
    glBindVertexArray(m_background.vao);
    glBindTextureUnit(0, m_background.sprite.texture);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 4 float per vertex in sprite VAO !!!
    glBindTextureUnit(0, 0);

//  Draw outline
    modelView = m_outline.transform.getMatrix();
    result = glms_mul(MVP, modelView);
    camera.updateUniformBuffer(result.raw);

    glUseProgram(m_outline.program);
    glBindVertexArray(m_outline.vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, m_outline.count); // 2 float per vertex in outline VAO !!!
    glBindVertexArray(0);
}


void PickHouse::resize(int width, int height) noexcept
{
    vec2 size = { static_cast<float>(width), static_cast<float>(height) };
    setSpriteSizeInPixels(m_background.sprite, size, m_background.transform);

    float dx = size[0] / m_background.sprite.width;
    float dy = size[1] / m_background.sprite.height;
    float outlinePositionX = 0;

    switch (m_selectedHouse)
    {
        case HouseType::ATREIDES:
            outlinePositionX = ATREIDES_OUTLINE_POSITION_X * dx;
        break;

        case HouseType::ORDOS:
            outlinePositionX = ORDOS_OUTLINE_POSITION_X * dx;
        break;

        case HouseType::HARKONNEN:
            outlinePositionX = HARKONNEN_OUTLINE_POSITION_X * dx;
        break;
        
        default:
            break;
    }

    m_outline.transform.setPosition(outlinePositionX, OUTLINE_POSITION_Y * dy);
    m_outline.transform.setScale(dx, dy);
}