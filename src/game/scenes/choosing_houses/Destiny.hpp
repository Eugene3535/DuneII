#ifndef DESTINY_HPP
#define DESTINY_HPP

#include <memory>

#include "common/Enums.hpp"
#include "resources/ogl/shaders/ShaderProgram.hpp"
#include "graphics/sprites/SpriteManager.hpp"
#include "graphics/outline/Outline.hpp"
#include "game/scenes/Scene.hpp"


class Destiny:
    public Scene
{
public:
    explicit Destiny(class DuneII* game) noexcept;
    ~Destiny();

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw()                        noexcept override;
    void resize(int width, int height) noexcept override;

    void press(int key) noexcept override;

private:
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_texture;

    ShaderProgram m_spriteProgram;
    ShaderProgram m_outlineProgram;
    
    SpriteManager m_sprites;

    Sprite m_background;
    Transform2D m_backgroundTransform;

    std::unique_ptr<Outline> m_outline;
    Transform2D m_outlineTransform;

    HouseType m_selectedHouse;
    float m_timer;
    bool m_outlineNeedUpdate;
};

#endif // !DESTINY_HPP