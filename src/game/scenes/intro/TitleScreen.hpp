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

    bool load(std::string_view info)    noexcept override;
    void update(float dt)               noexcept override;
    void draw()                         noexcept override;
    void resize(const glm::ivec2& size) noexcept override;

private:
    std::unique_ptr<SpriteManager> m_sprites;
    GLBuffer m_uniformBuffer;
    VertexArrayObject m_vao;

    ShaderProgram m_spriteProgram;
    Sprite m_space;
    Transform2D m_spaceTransform;
    Sprite m_planet;
    Transform2D m_planetTransform;

    Button* m_playButton;
    Button* m_exitButton;
    Button* m_settingsButton;

    char m_memoryPool[sizeof(Button) * 3];
};

#endif // !TITLE_SCREEN_HPP