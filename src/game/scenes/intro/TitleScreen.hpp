#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include "graphics/sprites/SpriteManager.hpp"
#include "game/scenes/intro/interactive_elements/Button.hpp"
#include "game/scenes/Scene.hpp"


class TitleScreen:
    public Scene
{
public:
    explicit TitleScreen(class DuneII* game) noexcept;
    ~TitleScreen();

    bool load(std::string_view info)         noexcept override;
    void update(float dt)                    noexcept override;
    void draw()                              noexcept override;
    void resize(int width, int height)       noexcept override;

private:
    GLuint m_textures[5];

    GLuint m_spriteProgram;
    GLuint m_buttonSpriteProgram;

    mesh::Sprite m_space;
    Transform2D  m_spaceTransform;
    mesh::Sprite m_planet;
    Transform2D  m_planetTransform;

    SpriteManager m_sprites;
    Button*       m_playButton;
    Button*       m_exitButton;
    Button*       m_settingsButton;
    char m_memoryPool[sizeof(Button) * 3];

    bool m_isPresented;
};

#endif // !TITLE_SCREEN_HPP