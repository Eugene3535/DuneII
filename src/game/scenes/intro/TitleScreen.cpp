#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "resources/files/FileProvider.hpp"
#include "resources/files/Shader.hpp"
#include "resources/ogl/texture/Texture.hpp"
#include "graphics/sprites/SpriteManager.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/intro/TitleScreen.hpp"


// Sprite scaling and position factors when changing window size
#define PLANET_SCALE_FACTOR_WIDTH  2.f
#define PLANET_SCALE_FACTOR_HEIGHT 1.5f
#define PLANET_POSITION_FACTOR_X   1.3f
#define PLANET_POSITION_FACTOR_Y   3.f

#define BUTTON_SCALE_FACTOR_WIDTH  10.f
#define BUTTON_SCALE_FACTOR_HEIGHT 15.f
#define BUTTON_POSITION_FACTOR_X   2.f
#define BUTTON_POSITION_FACTOR_Y   1.2f

#define TEXT_INFO_SCALE_FACTOR_WIDTH  3.f
#define TEXT_INFO_SCALE_FACTOR_HEIGHT 20.f

#define INTRO_TEXT_PRESENTATION "Какие-то компании представляют"

// Stages in seconds
#define INTRO_TEXT_PRESENT_START_TIME   5.f;
#define INTRO_TEXT_PRESENT_END_TIME     15.f;
#define INTRO_TITLE_SCREEN_PRESENT_TIME 20.f;

static bool is_moving_planet_begin;
static bool is_moving_planet_end;
static bool is_show_planet;
static bool is_intro_active_phase_end;


TitleScreen::TitleScreen(DuneII* game) noexcept:
    Scene(game),
    m_playButton(nullptr),
    m_exitButton(nullptr),
    m_settingsButton(nullptr),
    m_isMouseButtonPressed(false),
    m_isPresented(false)
{
#ifdef DEBUG
    m_isPresented = true;
#endif
}


TitleScreen::~TitleScreen()
{
    m_playButton->~Button();
    m_exitButton->~Button();
    m_settingsButton->~Button();
}


bool TitleScreen::load(std::string_view info) noexcept
{
    if(!m_isLoaded)
    {
        FileProvider provider;
        auto& glResources = m_game->glResources;

        auto vboHandles     = glResources.create<GLBuffer, 2>();
        auto textureHandles = glResources.create<Texture, 5>();
        auto vaoHandles     = glResources.create<VertexArrayObject, 1>();

//  TODO: move uniform buffer to Game
        m_uniformBuffer = GLBuffer(vboHandles[0], GL_UNIFORM_BUFFER);
        m_uniformBuffer.create(sizeof(mat4), 1, nullptr, GL_DYNAMIC_DRAW);
        m_uniformBuffer.bindBufferRange(0, 0, sizeof(mat4));

//  Textures
        Texture spaceTexture    = {.handle = textureHandles[0] };
        Texture planetTexture   = {.handle = textureHandles[1] };  
        Texture playTexture     = {.handle = textureHandles[2] }; 
        Texture exitTexture     = {.handle = textureHandles[3] };
        Texture settingsTexture = {.handle = textureHandles[4] };
    
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

//  Shaders
        std::array<Shader, 2> shaders;

        if (!shaders[0].loadFromFile(provider.findPathToFile("title_screen.vert"), GL_VERTEX_SHADER))   
            return false;

        if (!shaders[1].loadFromFile(provider.findPathToFile("title_screen.frag"), GL_FRAGMENT_SHADER)) 
            return false;

        if (!m_spriteProgram.link(shaders)) 
            return false;

        if (!shaders[0].loadFromFile(provider.findPathToFile("title_screen_button.vert"), GL_VERTEX_SHADER))   
            return false;

        if (!shaders[1].loadFromFile(provider.findPathToFile("title_screen_button.frag"), GL_FRAGMENT_SHADER)) 
            return false;

        if (!m_colorSpriteProgram.link(shaders)) 
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
        m_planetTransform.setOrigin(planetTexture.width * 0.5f, planetTexture.height * 0.5f);

//  Buttons
        int32_t uniform = m_colorSpriteProgram.getUniformLocation("buttonColor");

        if(uniform == -1)
            return false;

        char* offset = m_memoryPool;

        for(const auto btn : { "play", "exit", "settings" })
        {
            Sprite sprite = m_sprites->getSprite(btn);
            Button* button = new (offset) Button(sprite, uniform);
            offset += sizeof(Button);

            if(strcmp(btn, "play") == 0)     m_playButton = button;
            if(strcmp(btn, "exit") == 0)     m_exitButton = button;
            if(strcmp(btn, "settings") == 0) m_settingsButton = button;
        }
        
        m_isLoaded = true;
    }

    return m_isLoaded;
}


void TitleScreen::update(float dt) noexcept
{
    if(!m_isLoaded)
        return;

    if(m_isPresented)
    {
        m_settingsButton->update(m_mousePosition, m_isMouseButtonPressed);
        m_playButton->update(m_mousePosition, m_isMouseButtonPressed);
        m_exitButton->update(m_mousePosition, m_isMouseButtonPressed);

        m_isMouseButtonPressed = false;
    }
    else
    {

    }
}


void TitleScreen::draw() noexcept
{
    auto& camera = m_game->camera;
    mat4 MVP, modelView, model;
    camera.getModelViewProjectionMatrix(MVP);

    glUseProgram(m_spriteProgram.getHandle());
    glBindVertexArray(m_vao.getHandle());

    m_spaceTransform.getMatrix(model);
    glmc_mat4_mul(MVP, model, modelView);
    m_uniformBuffer.update(0, sizeof(mat4), 1, static_cast<const void*>(modelView));

    glBindTexture(GL_TEXTURE_2D, m_space.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_space.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_planetTransform.getMatrix(model);
    glmc_mat4_mul(MVP, model, modelView);
    m_uniformBuffer.update(0, sizeof(mat4), 1, static_cast<const void*>(modelView));

    glBindTexture(GL_TEXTURE_2D, m_planet.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_planet.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

//  Draw buttons
    glUseProgram(m_colorSpriteProgram.getHandle());

    m_playButton->getMatrix(model);
    glmc_mat4_mul(MVP, model, modelView);
    m_uniformBuffer.update(0, sizeof(mat4), 1, static_cast<const void*>(modelView));
    m_playButton->draw();

    m_exitButton->getMatrix(model);
    glmc_mat4_mul(MVP, model, modelView);
    m_uniformBuffer.update(0, sizeof(mat4), 1, static_cast<const void*>(modelView));
    m_exitButton->draw();

    m_settingsButton->getMatrix(model);
    glmc_mat4_mul(MVP, model, modelView);
    m_uniformBuffer.update(0, sizeof(mat4), 1, static_cast<const void*>(modelView)); 
    m_settingsButton->draw();

    glBindVertexArray(0);
    glUseProgram(0);
}


void TitleScreen::resize(int width, int height) noexcept
{
    if(!m_isLoaded)
        return;
    
    vec2 size = { static_cast<float>(width), static_cast<float>(height) };
    setSpriteSizeInPixels(m_space, size, m_spaceTransform);

    if(m_isPresented)
    {
        {// space and planet
            vec2 planetScale = { size[0] / PLANET_SCALE_FACTOR_WIDTH, size[1] / PLANET_SCALE_FACTOR_HEIGHT };
            setSpriteSizeInPixels(m_planet, planetScale, m_planetTransform);
            m_planetTransform.setPosition(size[0] / PLANET_POSITION_FACTOR_X, size[1] / PLANET_POSITION_FACTOR_Y);
        }

        {// buttons (size)
            const float width = size[0] / BUTTON_SCALE_FACTOR_WIDTH;
            const float height = size[1] / BUTTON_SCALE_FACTOR_HEIGHT;

            m_settingsButton->resize(width, height);
            m_playButton->resize(width, width); // <- this is not a typo
            m_exitButton->resize(width, height);
        }

        {// buttons (positions)
            const float centerX = size[0] / BUTTON_POSITION_FACTOR_X;
            const float centerY = size[1] / BUTTON_POSITION_FACTOR_Y;
            const float offset = size[0] / BUTTON_SCALE_FACTOR_WIDTH * PLANET_POSITION_FACTOR_X;

            m_settingsButton->setPosition(centerX - offset, centerY);
            m_playButton->setPosition(centerX, centerY);
            m_exitButton->setPosition(centerX + offset, centerY);
        }
    }
}


void TitleScreen::click(int button) noexcept
{
    if(button == GLFW_MOUSE_BUTTON_LEFT)
        m_isMouseButtonPressed = true;
}


void TitleScreen::setCursorPosition(float x, float y) noexcept
{
    m_mousePosition[0] = x;
    m_mousePosition[1] = y;
}