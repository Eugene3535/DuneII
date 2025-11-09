#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "resources/files/FileProvider.hpp"
#include "resources/files/Shader.hpp"
#include "resources/ogl/texture/Texture.hpp"
#include "graphics/sprites/SpriteManager.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/intro/TitleScreen.hpp"


// Sprite scaling and position factors when changing window size
#define PLANET_SCALE_FACTOR_WIDTH 2.f
#define PLANET_SCALE_FACTOR_HEIGHT 1.5f
#define PLANET_POSITION_FACTOR_X 1.3f
#define PLANET_POSITION_FACTOR_Y 3.f

#define BUTTON_SCALE_FACTOR_WIDTH 10.f
#define BUTTON_SCALE_FACTOR_HEIGHT 15.f
#define BUTTON_POSITION_FACTOR_X 2.f
#define BUTTON_POSITION_FACTOR_Y 1.2f

#define TEXT_INFO_SCALE_FACTOR_WIDTH 3.f
#define TEXT_INFO_SCALE_FACTOR_HEIGHT 20.f

#define INTRO_TEXT_PRESENTATION "Какие-то компании представляют"


TitleScreen::TitleScreen(DuneII* game) noexcept:
    Scene(game),
    m_spaceTexture(0),
    m_isPresented(false)
{
#ifdef DEBUG
    m_isPresented = true;
#endif
}


TitleScreen::~TitleScreen()
{
    // auto& glResources = m_game->glResources;
}


bool TitleScreen::load(std::string_view info) noexcept
{
    if(!m_isLoaded)
    {
        auto windowSize = m_game->getWindowSize();

        FileProvider provider;
        auto& glResources = m_game->glResources;

        auto vboHandles     = glResources.create<GLBuffer, 2>();
        auto textureHandles = glResources.create<Texture, 1>();
        auto vaoHandles     = glResources.create<VertexArrayObject, 1>();

        m_uniformBuffer = GLBuffer(vboHandles[0], GL_UNIFORM_BUFFER);
        m_uniformBuffer.create(sizeof(glm::mat4), 1, nullptr, GL_DYNAMIC_DRAW);
        m_uniformBuffer.bindBufferRange(0, 0, sizeof(glm::mat4));
        m_camera.setupProjectionMatrix(windowSize.x, windowSize.y);

        m_sprites = std::make_unique<SpriteManager>(vboHandles[1]);
        
        {// Space
            m_spaceTexture.handle = textureHandles[0];

            if(!m_spaceTexture.loadFromFile(provider.findPathToFile(SPACE_JPG)))
                return false;

            m_vao.setup(vaoHandles[0]);
        
            const std::array<VertexBufferLayout::Attribute, 1> spriteAttributes
            {
                VertexBufferLayout::Attribute::Float4
            };

            m_vao.addVertexBuffer(m_sprites->getVertexBuffer(), spriteAttributes);

            std::array<Shader, 2> shaders;

            if (!shaders[0].loadFromFile(provider.findPathToFile("sprite.vert"), GL_VERTEX_SHADER))   
                return false;

            if (!shaders[1].loadFromFile(provider.findPathToFile("sprite.frag"), GL_FRAGMENT_SHADER)) 
                return false;

            if (!m_spriteProgram.link(shaders)) 
                return false;

            m_sprites->createSprite("space", m_spaceTexture);

            if(auto spriteList = m_sprites->getSprites("space"); !spriteList.empty())
                m_space = spriteList[0];
        }


        m_isLoaded = true;
    }

    return m_isLoaded;
}


void TitleScreen::update(float dt) noexcept
{

}


void TitleScreen::draw() noexcept
{
    glm::mat4 mvp = m_camera.getModelViewProjectionMatrix() * m_spaceTransform.getMatrix();
    m_uniformBuffer.update(0, sizeof(glm::mat4), 1, static_cast<const void*>(glm::value_ptr(mvp)));

    glUseProgram(m_spriteProgram.getHandle());
    glBindVertexArray(m_vao.getHandle());

    glBindTexture(GL_TEXTURE_2D, m_space.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_space.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(m_vao.getHandle());
    glUseProgram(0);
}


void TitleScreen::resize(const glm::ivec2& size) noexcept
{
    m_camera.setupProjectionMatrix(size.x, size.y);
    setSpriteSizeInPixels(m_space, glm::vec2(size), m_spaceTransform);
}
