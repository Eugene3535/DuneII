#pragma once

#include <array>
#include <memory>

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
    std::unique_ptr<Button> m_playButton;
    std::unique_ptr<Button> m_exitButton;
    std::unique_ptr<Button> m_settingsButton;

    bool m_isPresented;
};