#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "resources/files/FileProvider.hpp"
#include "resources/files/Shader.hpp"
#include "resources/gl_interfaces/texture/Texture.hpp"
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
    m_mousePosition(glms_vec2_zero()),
    m_isMouseButtonPressed(false),
    m_isPresented(true)
{

}


TitleScreen::~TitleScreen()
{
    if(m_playButton)
        m_playButton->~Button();

    if(m_exitButton)
        m_exitButton->~Button();

    if(m_settingsButton)
        m_settingsButton->~Button();

    glDeleteTextures(5, m_textures);
}


bool TitleScreen::load(std::string_view info) noexcept
{
    if(m_isLoaded)
        return true;
    
    auto& provider = m_game->fileProvider;
    
    glGenTextures(5, m_textures);

//  Textures
    Texture spaceTexture    = {.handle = m_textures[0] };
    Texture planetTexture   = {.handle = m_textures[1] };  
    Texture playTexture     = {.handle = m_textures[2] }; 
    Texture exitTexture     = {.handle = m_textures[3] };
    Texture settingsTexture = {.handle = m_textures[4] };

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
    {
        std::array<Shader, 2> shaders;

        if(!shaders[0].loadFromFile(provider.findPathToFile("sprite.vert"), GL_VERTEX_SHADER))
            return false;

        if(!shaders[1].loadFromFile(provider.findPathToFile("sprite.frag"), GL_FRAGMENT_SHADER))
            return false;

        if(!m_spriteProgram.link(shaders))
            return false;

        if(!shaders[0].loadFromFile(provider.findPathToFile("color_sprite.vert"), GL_VERTEX_SHADER))
            return false;

        if(!shaders[1].loadFromFile(provider.findPathToFile("color_sprite.frag"), GL_FRAGMENT_SHADER))
            return false;

        if(!m_buttonSpriteProgram.link(shaders) )
            return false;
    }
    
//  Sprites
    m_sprites.createSprite("space", spaceTexture);
    m_sprites.createSprite("planet", planetTexture);
    m_sprites.createSprite("play", playTexture);
    m_sprites.createSprite("exit", exitTexture);
    m_sprites.createSprite("settings", settingsTexture);

    if(auto spaceSprite = m_sprites.getSprite("space"); spaceSprite.has_value())
        m_space = spaceSprite.value();

    if(auto planetSprite = m_sprites.getSprite("planet"); planetSprite.has_value())
        m_planet = planetSprite.value();

    m_planetTransform.setOrigin(planetTexture.width * 0.5f, planetTexture.height * 0.5f);

//  Buttons
    int32_t uniform = glGetUniformLocation(m_buttonSpriteProgram.getHandle(), "spriteColor");

    if(uniform == -1)
        return false;

    char* offset = m_memoryPool;

    for(const auto btn : { "play", "exit", "settings" })
    {
        if(auto sprite = m_sprites.getSprite(btn); sprite.has_value())
        {
            Button* button = new(offset) Button(sprite.value(), uniform);
            offset += sizeof(Button);

            if(strcmp(btn, "play") == 0)     m_playButton = button;
            if(strcmp(btn, "exit") == 0)     m_exitButton = button;
            if(strcmp(btn, "settings") == 0) m_settingsButton = button;
        }
    }
    
    m_isLoaded = true;
    
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

        if(m_playButton->isSelected())
            m_game->switchScene(this, Scene::PICK_HOUSE);
    }
    else
    {

    }
}


void TitleScreen::draw() noexcept
{
    if(!m_isLoaded)
        return;
        
    alignas(16) mat4 MVP;
    alignas(16) mat4 modelView;
    alignas(16) mat4 model;
    
    auto& camera = m_game->camera;
    camera.getModelViewProjectionMatrix(MVP);

    m_spriteProgram(true);
    m_sprites.bind(true);

    m_spaceTransform.calculate(model);
    glmc_mat4_mul(MVP, model, modelView);
    camera.updateUniformBuffer(modelView);

    glBindTexture(GL_TEXTURE_2D, m_space.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_space.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_planetTransform.calculate(model);
    glmc_mat4_mul(MVP, model, modelView);
    camera.updateUniformBuffer(modelView);

    glBindTexture(GL_TEXTURE_2D, m_planet.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_planet.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

//  Draw buttons
    if(m_isPresented)
    {
        m_buttonSpriteProgram(true);

        m_playButton->calculate(model);
        glmc_mat4_mul(MVP, model, modelView);
        camera.updateUniformBuffer(modelView);
        m_playButton->draw();

        m_exitButton->calculate(model);
        glmc_mat4_mul(MVP, model, modelView);
        camera.updateUniformBuffer(modelView);
        m_exitButton->draw();

        m_settingsButton->calculate(model);
        glmc_mat4_mul(MVP, model, modelView);
        camera.updateUniformBuffer(modelView);
        m_settingsButton->draw();
    }

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
    m_mousePosition = { x, y };
}