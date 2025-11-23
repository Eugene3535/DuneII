#include "common/Enums.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/choosing_houses/Destiny.hpp"


#define ATREIDES_OUTLINE_POSITION_X 32.f
#define ORDOS_OUTLINE_POSITION_X 120.f
#define HARKONNEN_OUTLINE_POSITION_X 208.f
#define OUTLINE_POSITION_Y 136.f

#define DEFAULT_OUTLINE_SIZE_X 80.f
#define DEFAULT_OUTLINE_SIZE_Y 16.f


Destiny::Destiny(DuneII* game) noexcept:
    Scene(game),
    m_spriteProgram(0),
    m_sprites(game->glResources)
{

}


Destiny::~Destiny() = default;


bool Destiny::load(std::string_view info) noexcept
{
    if(m_isLoaded)
        return true;

    auto& provider = m_game->fileProvider;
    auto& glResources = m_game->glResources;

    auto vboHandles     = glResources.create<GLBuffer, 1>();
    auto textureHandles = glResources.create<Texture, 1>();
    auto vaoHandles     = glResources.create<VertexArrayObject, 1>();

//  Textures
    Texture housesTexture = {.handle = textureHandles[0] };

    if(!housesTexture.loadFromFile(provider.findPathToFile(HOUSES_PNG)))
        return false;

//  Shaders
    if(m_spriteProgram = glResources.getShaderProgram("title_screen"); m_spriteProgram == 0)
        return false;

//  Sprites
    m_sprites.createSprite("houses", housesTexture);
    m_houses = m_sprites.getSprite("houses");

    m_isLoaded = true;

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

    m_transform.calculate(modelView);
    glmc_mat4_mul(MVP, modelView, result);
    m_game->updateUniformBuffer(result);

    glBindTexture(GL_TEXTURE_2D, m_houses.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_houses.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}


void Destiny::resize(int width, int height) noexcept
{
    vec2 size = { static_cast<float>(width), static_cast<float>(height) };
    setSpriteSizeInPixels(m_houses, size, m_transform);
}