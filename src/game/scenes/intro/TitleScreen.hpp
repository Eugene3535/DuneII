#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include <memory>

#include "resources/ogl/vao/VertexArrayObject.hpp"
#include "resources/ogl/shaders/ShaderProgram.hpp"
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
    void click(int button)                   noexcept override;
    void setCursorPosition(float x, float y) noexcept override;

private:
    VertexArrayObject m_vao;

    ShaderProgram m_spriteProgram;
    ShaderProgram m_buttonSpriteProgram;

    Sprite m_space;
    Transform2D m_spaceTransform;
    Sprite m_planet;
    Transform2D m_planetTransform;

    SpriteManager* m_sprites;
    Button*        m_playButton;
    Button*        m_exitButton;
    Button*        m_settingsButton;
    char m_memoryPool[sizeof(SpriteManager) + sizeof(Button) * 3];

    vec2 m_mousePosition;
    bool m_isMouseButtonPressed;

    bool m_isPresented;
};

#endif // !TITLE_SCREEN_HPP