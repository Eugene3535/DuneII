#include <cstring>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "cglm/struct/affine-mat.h"

#include "files/FileProvider.hpp"
#include "graphics/texture/Texture2D.hpp"
#include "application/game/Game.hpp"
#include "scenes/intro/TitleScreen.hpp"


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

// Stages in seconds
#define INTRO_TEXT_PRESENT_START_TIME   5.f;
#define INTRO_TEXT_PRESENT_END_TIME     15.f;
#define INTRO_TITLE_SCREEN_PRESENT_TIME 20.f;

static bool is_moving_planet_begin;
static bool is_moving_planet_end;
static bool is_show_planet;
static bool is_intro_active_phase_end;


TitleScreen::TitleScreen(Game* game) noexcept:
    Scene(game, Scene::MAIN_MENU),
    m_spriteProgram(0),
    m_buttonSpriteProgram(0),
    m_playButton(nullptr),
    m_exitButton(nullptr),
    m_settingsButton(nullptr),
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

    glDeleteTextures(static_cast<GLsizei>(m_textures.size()), m_textures.data());
}


bool TitleScreen::load(std::string_view info) noexcept
{
    if (m_isLoaded)
        return true;
    
    glCreateTextures(GL_TEXTURE_2D, static_cast<GLsizei>(m_textures.size()), m_textures.data());

//  Textures
    Texture2D spaceTexture(m_textures[0]);
    Texture2D planetTexture(m_textures[1]);  
    Texture2D playTexture(m_textures[2]); 
    Texture2D exitTexture(m_textures[3]);
    Texture2D settingsTexture(m_textures[4]);

    if (!spaceTexture.loadFromFile(FileProvider::findPathToFile(SPACE_JPG)))
        return false;

    if (!planetTexture.loadFromFile(FileProvider::findPathToFile(DUNE_PNG)))
        return false;

    if (!playTexture.loadFromFile(FileProvider::findPathToFile(BUTTON_PLAY_PNG)))
        return false;

    if (!exitTexture.loadFromFile(FileProvider::findPathToFile(BUTTON_EXIT_RU_PNG)))
        return false;

    if (!settingsTexture.loadFromFile(FileProvider::findPathToFile(BUTTON_SETTINGS_RU_PNG)))
        return false;

//  Shaders
    {
        if (m_spriteProgram = m_game->getShaderProgram("sprite"); m_spriteProgram == 0)
            return false;

        if (m_buttonSpriteProgram = m_game->getShaderProgram("color_sprite"); m_buttonSpriteProgram == 0)
            return false;
    }
    
//  Sprites
    m_sprites.createSprite("space", spaceTexture.getHandle());
    m_sprites.createSprite("planet", planetTexture.getHandle());
    m_sprites.createSprite("play", playTexture.getHandle());
    m_sprites.createSprite("exit", exitTexture.getHandle());
    m_sprites.createSprite("settings", settingsTexture.getHandle());
    m_sprites.pushVerticesOnGPU();

    if (auto spaceSprite = m_sprites.getSprite("space"); spaceSprite.has_value())
        m_space = spaceSprite.value();

    if (auto planetSprite = m_sprites.getSprite("planet"); planetSprite.has_value())
        m_planet = planetSprite.value();

    auto texSize = planetTexture.getSize();
    m_planetTransform.setOrigin(texSize.x * 0.5f, texSize.y * 0.5f);

//  Buttons
    int32_t uniform = glGetUniformLocation(m_buttonSpriteProgram, "spriteColor");

    if (uniform == -1)
        return false;

    char* offset = m_memoryPool;

    for (const auto btn : { "play", "exit", "settings" })
    {
        if (auto sprite = m_sprites.getSprite(btn); sprite.has_value())
        {
            Button* button = new(offset) Button(sprite.value(), uniform);
            offset += sizeof(Button);

            if (strcmp(btn, "play") == 0)     m_playButton = button;
            if (strcmp(btn, "exit") == 0)     m_exitButton = button;
            if (strcmp(btn, "settings") == 0) m_settingsButton = button;
        }
    }
    
    m_isLoaded = true;
    
    return m_isLoaded;
}


void TitleScreen::update(float dt) noexcept
{
    if (!m_isLoaded)
        return;




    if (!m_isPresented)
        return;
    
    const bool isMouseButtonPressed = ( (m_mouse.button == GLFW_MOUSE_BUTTON_LEFT) && (m_mouse.action == GLFW_PRESS) );

    m_settingsButton->update(m_cursor, isMouseButtonPressed);
    m_playButton->update(m_cursor, isMouseButtonPressed);
    m_exitButton->update(m_cursor, isMouseButtonPressed);

    if(m_playButton->isSelected())
        m_game->switchScene(this, Scene::PICK_HOUSE);
}


void TitleScreen::draw(const mat4s& projection) noexcept
{
    if(!m_isLoaded)
        return;
        
    mat4s MVP = projection;
    mat4s modelView;
    mat4s model;

    glUseProgram(m_spriteProgram);
    m_sprites.bind(true);

    model = m_spaceTransform.getMatrix();
    modelView = glms_mul(MVP, model);
    m_game->updateUniformBuffer(modelView);

    glBindTexture(GL_TEXTURE_2D, m_space.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_space.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    model = m_planetTransform.getMatrix();
    modelView = glms_mul(MVP, model);
    m_game->updateUniformBuffer(modelView);

    glBindTexture(GL_TEXTURE_2D, m_planet.texture);
    glDrawArrays(GL_TRIANGLE_FAN, m_planet.frame, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

//  Draw buttons
    if(m_isPresented)
    {
        glUseProgram(m_buttonSpriteProgram);

        model = m_playButton->getMatrix();
        modelView = glms_mul(MVP, model);
        m_game->updateUniformBuffer(modelView);
        m_playButton->draw();

        model = m_exitButton->getMatrix();
        modelView = glms_mul(MVP, model);
        m_game->updateUniformBuffer(modelView);
        m_exitButton->draw();

        model = m_settingsButton->getMatrix();
        modelView = glms_mul(MVP, model);
        m_game->updateUniformBuffer(modelView);
        m_settingsButton->draw();
    }

    glBindVertexArray(0);
    glUseProgram(0);
}


void TitleScreen::resize(int width, int height) noexcept
{
    Scene::resize(width, height);

    if (!m_isLoaded)
        return;
        
    vec2s size = { static_cast<float>(width), static_cast<float>(height) };
    setSpriteSizeInPixels(m_space, size, m_spaceTransform);

    if (!m_isPresented)
        return;
    
    {// space and planet
        vec2s planetScale = { size.x / PLANET_SCALE_FACTOR_WIDTH, size.y / PLANET_SCALE_FACTOR_HEIGHT };
        setSpriteSizeInPixels(m_planet, planetScale, m_planetTransform);
        m_planetTransform.setPosition(size.x / PLANET_POSITION_FACTOR_X, size.y / PLANET_POSITION_FACTOR_Y);
    }

    {// buttons (size)
        const float width = size.x / BUTTON_SCALE_FACTOR_WIDTH;
        const float height = size.y / BUTTON_SCALE_FACTOR_HEIGHT;

        m_settingsButton->resize(width, height);
        m_playButton->resize(width, width); // <- this is not a typo
        m_exitButton->resize(width, height);
    }

    {// buttons (positions)
        const float centerX = size.x / BUTTON_POSITION_FACTOR_X;
        const float centerY = size.y / BUTTON_POSITION_FACTOR_Y;
        const float offset = size.x / BUTTON_SCALE_FACTOR_WIDTH * PLANET_POSITION_FACTOR_X;

        m_settingsButton->setPosition(centerX - offset, centerY);
        m_playButton->setPosition(centerX, centerY);
        m_exitButton->setPosition(centerX + offset, centerY);
    }
}