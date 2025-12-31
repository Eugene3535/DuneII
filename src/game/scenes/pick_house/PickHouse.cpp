#include <cstring>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "cglm/struct/affine-mat.h"

#include "resources/files/FileProvider.hpp"
#include "resources/files/Shader.hpp"
#include "resources/gl_interfaces/texture/Texture.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/pick_house/PickHouse.hpp"


#define ATREIDES_OUTLINE_POSITION_X 32.f
#define ORDOS_OUTLINE_POSITION_X 120.f
#define HARKONNEN_OUTLINE_POSITION_X 208.f
#define OUTLINE_POSITION_Y 136.f

#define DEFAULT_OUTLINE_WIDTH 80.f
#define DEFAULT_OUTLINE_HEIGHT 16.f
#define SWITCH_HOUSE_OUTLINE_DELAY 0.3f


PickHouse::PickHouse(DuneII* game) noexcept:
    Scene(game, Scene::PICK_HOUSE),
    m_vbo(0),
    m_vao(0),
    m_texture(0),
    m_spriteProgram(0),
    m_outlineProgram(0),
    m_backgroundTransform(),
    m_outlineTransform(),
    m_selectedHouse(HouseType::ATREIDES),
    m_timer(0.f),
    m_outlineNeedUpdate(true)
{
    
}


PickHouse::~PickHouse()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteTextures(1, &m_texture);
}


bool PickHouse::load(std::string_view info) noexcept
{
    if(m_isLoaded)
        return true;

    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);
    glGenTextures(1, &m_texture);

//  Textures
    Texture housesTexture = {.handle = m_texture };

    if(!housesTexture.loadFromFile(FileProvider::findPathToFile(HOUSES_PNG)))
        return false;

//  Shaders
    {
        if(m_spriteProgram = m_game->getShaderProgram("sprite"); m_spriteProgram == 0)
            return false;

        if(m_outlineProgram = m_game->getShaderProgram("color_outline"); m_outlineProgram == 0)
            return false;
    }

    if(GLint uniformColor = glGetUniformLocation(m_outlineProgram, "outlineColor"); uniformColor != -1)
    {
        const float outlineColor[] = { 1.f, 0.f, 0.f, 1.f };
        glUseProgram(m_outlineProgram);
        glUniform4fv(uniformColor, 1, outlineColor);
        glUseProgram(0); 
    }

//  Sprites
    memset(&m_background, 0, sizeof(Sprite));
    m_sprites.createSprite("background", housesTexture);

    if(auto bg = m_sprites.getSprite("background"); bg.has_value())
        m_background = bg.value();

//  Outline
    m_outline = std::make_unique<Outline>(m_vbo, m_vao);
    const vec2s outlineSize = { DEFAULT_OUTLINE_WIDTH, DEFAULT_OUTLINE_HEIGHT };

    m_outline->create(4, [outlineSize](size_t index) -> vec2s
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

    m_isLoaded = (m_background.texture != 0);

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
                if (m_game->isKeyPressed(GLFW_KEY_RIGHT))
                    m_selectedHouse = HouseType::ORDOS;
                break;

            case HouseType::ORDOS:
                if (m_game->isKeyPressed(GLFW_KEY_LEFT))
                    m_selectedHouse = HouseType::ATREIDES;

                if (m_game->isKeyPressed(GLFW_KEY_RIGHT))
                    m_selectedHouse = HouseType::HARKONNEN;
                break;

            case HouseType::HARKONNEN:
                if (m_game->isKeyPressed(GLFW_KEY_LEFT))
                    m_selectedHouse = HouseType::ORDOS;
                break;

            default:
                break;
        }
    }

    if (m_game->isKeyPressed(GLFW_KEY_ENTER))
        m_game->switchScene(this, Scene::MISSION);

    if(m_outlineNeedUpdate)
    {
        auto windowSize = m_game->getWindowsSize();
        vec2 size = { static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) };

        float dx = size[0] / m_background.width;
        float dy = size[1] / m_background.height;
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

        m_outlineTransform.setPosition(outlinePositionX, OUTLINE_POSITION_Y * dy);
        m_outlineNeedUpdate = false;
    }
}


void PickHouse::draw() noexcept
{
    if(!m_isLoaded)
        return;
        
    alignas(16) mat4s MVP;
    alignas(16) mat4s modelView;
    alignas(16) mat4s result;

    auto& camera = m_game->camera;
    camera.getModelViewProjectionMatrix(MVP.raw);

    glUseProgram(m_spriteProgram);
    m_sprites.bind(true);

    modelView = m_backgroundTransform.getMatrix();
    result = glms_mul(MVP, modelView);
    camera.updateUniformBuffer(result.raw);

    glBindTexture(GL_TEXTURE_2D, m_background.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_background.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_outlineProgram);

    modelView = m_outlineTransform.getMatrix();
    result = glms_mul(MVP, modelView);
    camera.updateUniformBuffer(result.raw);
    m_outline->draw();

    glUseProgram(0);
}


void PickHouse::resize(int width, int height) noexcept
{
    vec2 size = { static_cast<float>(width), static_cast<float>(height) };
    setSpriteSizeInPixels(m_background, size, m_backgroundTransform);

    float dx = size[0] / m_background.width;
    float dy = size[1] / m_background.height;
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

    m_outlineTransform.setPosition(outlinePositionX, OUTLINE_POSITION_Y * dy);
    m_outlineTransform.setScale(dx, dy);
}