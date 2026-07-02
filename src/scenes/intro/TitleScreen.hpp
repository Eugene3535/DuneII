#pragma once

#include <array>

#include "scenes/intro/interactive_elements/Button.hpp"
#include "scenes/Scene.hpp"


class TitleScreen:
    public Scene
{
public:
    explicit TitleScreen(class Game* game) noexcept;
    ~TitleScreen();

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw(const mat4s& projection) noexcept override;
    void resize(int width, int height) noexcept override;

private:
    std::array<GLuint, 5> m_textures;

    GLuint m_spriteProgram;
    GLuint m_buttonSpriteProgram;

    Sprite2D    m_space;
    Transform2D m_spaceTransform;
    Sprite2D    m_planet;
    Transform2D m_planetTransform;

    SpriteManager m_sprites;
    Button*       m_playButton;
    Button*       m_exitButton;
    Button*       m_settingsButton;
    char m_memoryPool[sizeof(Button) * 3];

    bool m_isPresented;
};