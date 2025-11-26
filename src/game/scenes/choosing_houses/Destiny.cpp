#include <cstring>

#include "common/Enums.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/choosing_houses/Destiny.hpp"


#define ATREIDES_OUTLINE_POSITION_X 32.f
#define ORDOS_OUTLINE_POSITION_X 120.f
#define HARKONNEN_OUTLINE_POSITION_X 208.f
#define OUTLINE_POSITION_Y 136.f

#define DEFAULT_OUTLINE_WIDTH 80.f
#define DEFAULT_OUTLINE_HEIGHT 16.f

HouseType current_house = HouseType::ATREIDES;

static void set_outline_params(const Sprite& background, vec2 newSize, Transform2D& transform) noexcept
{
    float dx = newSize[0] / background.width;
    float dy = newSize[1] / background.height;
    float outlinePositionX = 0;

    switch (current_house)
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

    transform.setPosition(outlinePositionX, OUTLINE_POSITION_Y * dy);
    transform.setScale(dx, dy);
}


Destiny::Destiny(DuneII* game) noexcept:
    Scene(game),
    m_spriteProgram(0),
    m_outlineProgram(0),
    m_sprites(game->glResources),
    m_backgroundTransform(),
    m_outlineTransform()
{
    
}


bool Destiny::load(std::string_view info) noexcept
{
    if(m_isLoaded)
        return true;

    auto& provider = m_game->fileProvider;
    auto& glResources = m_game->glResources;

    auto vboHandles     = glResources.create<GLBuffer, 1>();
    auto vaoHandles     = glResources.create<VertexArrayObject, 1>();
    auto textureHandles = glResources.create<Texture, 1>();

//  Textures
    Texture housesTexture = {.handle = textureHandles[0] };

    if(!housesTexture.loadFromFile(provider.findPathToFile(HOUSES_PNG)))
        return false;

//  Shaders
    if(m_spriteProgram = glResources.getShaderProgram("sprite"); m_spriteProgram == 0)
        return false;
    
    if(m_outlineProgram = glResources.getShaderProgram("color_outline"); m_outlineProgram == 0)
        return false;

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
    m_background = m_sprites.getSprite("background");

//  Outline
    m_outline = std::make_unique<Outline>(vboHandles[0], vaoHandles[0]);
    vec2s outlineSize = { DEFAULT_OUTLINE_WIDTH, DEFAULT_OUTLINE_HEIGHT };

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


void Destiny::update(float dt) noexcept
{

}


void Destiny::draw() noexcept
{
    if(!m_isLoaded)
        return;
        
    alignas(16) mat4 MVP;
    alignas(16) mat4 modelView;
    alignas(16) mat4 result;

    auto& camera = m_game->camera;
    camera.getModelViewProjectionMatrix(MVP);

    glUseProgram(m_spriteProgram);
    m_sprites.bind(true);

    m_backgroundTransform.calculate(modelView);
    glmc_mat4_mul(MVP, modelView, result);
    m_game->updateUniformBuffer(result);

    glBindTexture(GL_TEXTURE_2D, m_background.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_background.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_outlineProgram);

    m_outlineTransform.calculate(modelView);
    glmc_mat4_mul(MVP, modelView, result);
    m_game->updateUniformBuffer(result);
    m_outline->draw();

    glUseProgram(0);
}


void Destiny::resize(int width, int height) noexcept
{
    vec2 size = { static_cast<float>(width), static_cast<float>(height) };
    setSpriteSizeInPixels(m_background, size, m_backgroundTransform);
    set_outline_params(m_background, size, m_outlineTransform);
}