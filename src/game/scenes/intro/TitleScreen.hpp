#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include <memory>

#include "resources/ogl/texture/Texture.hpp"
#include "resources/ogl/vao/VertexArrayObject.hpp"
#include "resources/ogl/shaders/ShaderProgram.hpp"
#include "graphics/Meshes.hpp"
#include "graphics/Transform2D.hpp"
#include "graphics/sprites/SpriteManager.hpp"
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

    Texture m_spaceTexture;
    VertexArrayObject m_vao;
    ShaderProgram m_spriteProgram;
    Sprite m_space;
    Transform2D m_spaceTransform;

    bool m_isPresented;
};

#endif // !TITLE_SCREEN_HPP