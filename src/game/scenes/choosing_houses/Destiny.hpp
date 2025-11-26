#ifndef DESTINY_HPP
#define DESTINY_HPP

#include <memory>

#include "resources/ogl/vao/VertexArrayObject.hpp"
#include "graphics/sprites/SpriteManager.hpp"
#include "graphics/outline/Outline.hpp"
#include "game/scenes/Scene.hpp"


class Destiny:
    public Scene
{
public:
    explicit Destiny(class DuneII* game) noexcept;

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw()                        noexcept override;
    void resize(int width, int height) noexcept override;

private:
    GLuint m_spriteProgram;
    GLuint m_outlineProgram;
    SpriteManager m_sprites;

    Sprite m_background;
    Transform2D m_backgroundTransform;

    std::unique_ptr<Outline> m_outline;
    Transform2D m_outlineTransform;
};

#endif // !DESTINY_HPP