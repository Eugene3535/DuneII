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
    m_playButton(nullptr),
    m_exitButton(nullptr),
    m_settingsButton(nullptr)
{

}


TitleScreen::~TitleScreen()
{
    // auto& glResources = m_game->glResources;
}


bool TitleScreen::load(std::string_view info) noexcept
{
    if(!m_isLoaded)
    {
        const auto windowSize = m_game->getWindowSize();

        FileProvider provider;
        auto& glResources = m_game->glResources;

        auto vboHandles     = glResources.create<GLBuffer, 2>();
        auto textureHandles = glResources.create<Texture, 5>();
        auto vaoHandles     = glResources.create<VertexArrayObject, 1>();

//  TODO: move uniform buffer to Game
        m_uniformBuffer = GLBuffer(vboHandles[0], GL_UNIFORM_BUFFER);
        m_uniformBuffer.create(sizeof(glm::mat4), 1, nullptr, GL_DYNAMIC_DRAW);
        m_uniformBuffer.bindBufferRange(0, 0, sizeof(glm::mat4));

//  Textures
        Texture spaceTexture;    spaceTexture.handle    = textureHandles[0];
        Texture planetTexture;   planetTexture.handle   = textureHandles[1];
        Texture playTexture;     playTexture.handle     = textureHandles[2];
        Texture exitTexture;     exitTexture.handle     = textureHandles[3];
        Texture settingsTexture; settingsTexture.handle = textureHandles[4];
    
        if(!spaceTexture.loadFromFile(provider.findPathToFile(SPACE_JPG)))
            return false;

        if(!planetTexture.loadFromFile(provider.findPathToFile(DUNE_PNG)))
            return false;

        if(!playTexture.loadFromFile(provider.findPathToFile(BUTTON_PLAY_PNG)))
            return false;

        if(!exitTexture.loadFromFile(provider.findPathToFile(BUTTON_EXIT_RU_PNG)))
            return false;

        if(!settingsTexture.loadFromFile(provider.findPathToFile(BUTTON_SETTINGS_RU_PNG)))
            return false;
        
//  Sprites
        m_vao.setup(vaoHandles[0]);
        m_sprites = std::make_unique<SpriteManager>(vboHandles[1]);

        const std::array<VertexBufferLayout::Attribute, 1> spriteAttributes
        {
            VertexBufferLayout::Attribute::Float4
        };

        m_vao.addVertexBuffer(m_sprites->getVertexBuffer(), spriteAttributes);

        m_sprites->createSprite("space", spaceTexture);
        m_sprites->createSprite("planet", planetTexture);
        m_sprites->createSprite("play", playTexture);
        m_sprites->createSprite("exit", exitTexture);
        m_sprites->createSprite("settings", settingsTexture);

        m_space = m_sprites->getSprite("space");
        m_planet = m_sprites->getSprite("planet");

//  Buttons
        char* offset = m_memoryPool;

        for(const auto btn : { "play", "exit", "settings" })
        {
            Sprite sprite = m_sprites->getSprite(btn);
            Button* button = new (offset) Button(sprite);
            offset += sizeof(Button);
        }

        offset = m_memoryPool;
        m_playButton = reinterpret_cast<Button*>(offset); offset += sizeof(Button);
        m_exitButton = reinterpret_cast<Button*>(offset); offset += sizeof(Button);
        m_settingsButton = reinterpret_cast<Button*>(offset);

        m_playButton->setPosition({ 100, 100 });
        m_exitButton->setPosition({ 350, 300 });
        m_settingsButton->setPosition({ 650, 500 });
        
//  Shaders
        std::array<Shader, 2> shaders;

        if (!shaders[0].loadFromFile(provider.findPathToFile("sprite.vert"), GL_VERTEX_SHADER))   
            return false;

        if (!shaders[1].loadFromFile(provider.findPathToFile("sprite.frag"), GL_FRAGMENT_SHADER)) 
            return false;

        if (!m_spriteProgram.link(shaders)) 
            return false;
        
        m_isLoaded = true;
    }

    return m_isLoaded;
}


void TitleScreen::update(float dt) noexcept
{

}


void TitleScreen::draw() noexcept
{
    auto& registry         = m_game->registry; 
    const auto camera      = m_game->camera;
    const auto& projection = registry.get<glm::mat4>(camera);
    const auto& modelView  = registry.get<Transform2D>(camera);
    const auto mvp         = projection * modelView.getMatrix();

    glUseProgram(m_spriteProgram.getHandle());
    glBindVertexArray(m_vao.getHandle());

    glm::mat4 modelTransform = mvp * m_spaceTransform.getMatrix();
    m_uniformBuffer.update(0, sizeof(glm::mat4), 1, static_cast<const void*>(glm::value_ptr(modelTransform)));

    glBindTexture(GL_TEXTURE_2D, m_space.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_space.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    modelTransform = mvp * m_planetTransform.getMatrix();
    m_uniformBuffer.update(0, sizeof(glm::mat4), 1, static_cast<const void*>(glm::value_ptr(modelTransform)));

    glBindTexture(GL_TEXTURE_2D, m_planet.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_planet.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    modelTransform = mvp * m_playButton->getTransform().getMatrix();
    m_uniformBuffer.update(0, sizeof(glm::mat4), 1, static_cast<const void*>(glm::value_ptr(modelTransform)));
    m_playButton->draw();

    modelTransform = mvp * m_exitButton->getTransform().getMatrix();
    m_uniformBuffer.update(0, sizeof(glm::mat4), 1, static_cast<const void*>(glm::value_ptr(modelTransform)));
    m_exitButton->draw();

    modelTransform = mvp * m_settingsButton->getTransform().getMatrix();
    m_uniformBuffer.update(0, sizeof(glm::mat4), 1, static_cast<const void*>(glm::value_ptr(modelTransform))); 
    m_settingsButton->draw();

    glBindVertexArray(m_vao.getHandle());
    glUseProgram(0);
}


void TitleScreen::resize(const glm::ivec2& size) noexcept
{
    Scene::resize(size);

    setSpriteSizeInPixels(m_space, glm::vec2(size), m_spaceTransform);
}
