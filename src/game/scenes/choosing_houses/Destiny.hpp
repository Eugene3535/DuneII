#ifndef DESTINY_HPP
#define DESTINY_HPP

#include "resources/ogl/vao/VertexArrayObject.hpp"
#include "graphics/sprites/SpriteManager.hpp"
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

private:
    GLuint m_spriteProgram;
    SpriteManager m_sprites;

    Sprite m_houses;
    Transform2D m_transform;
};

#endif // !DESTINY_HPP