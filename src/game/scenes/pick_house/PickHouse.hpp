#ifndef PICK_HOUSE_HPP
#define PICK_HOUSE_HPP

#include <memory>

#include "common/Enums.hpp"
#include "resources/gl_interfaces/shaders/ShaderProgram.hpp"
#include "graphics/sprites/SpriteManager.hpp"
#include "graphics/outline/Outline.hpp"
#include "game/scenes/Scene.hpp"


class PickHouse:
    public Scene
{
public:
    explicit PickHouse(class DuneII* game) noexcept;
    ~PickHouse();

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw()                        noexcept override;
    void resize(int width, int height) noexcept override;

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

#endif // !PICK_HOUSE_HPP