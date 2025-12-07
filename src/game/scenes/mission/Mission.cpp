#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "resources/files/Shader.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/mission/Mission.hpp"


Mission::Mission(DuneII* game) noexcept:
    Scene(game)
{

}


Mission::~Mission()
{
    glDeleteTextures(1, textures);
    glDeleteVertexArrays(1, vao);
    glDeleteBuffers(2, vbo);
}


bool Mission::load(std::string_view info) noexcept
{
    if(m_isLoaded)
        return true;
  
    auto& provider = m_game->fileProvider;

    glGenTextures(1, textures);
    glGenBuffers(2, vbo);
    glGenVertexArrays(1, vao);

    Texture landscapeTexture = {.handle = textures[0] };

    if(!landscapeTexture.loadFromFile(provider.findPathToFile(LANDSCAPE_PNG)))
        return false;

    {
        std::array<Shader, 2> shaders;

        if(!shaders[0].loadFromFile(provider.findPathToFile("tilemap.vert"), GL_VERTEX_SHADER))
            return false;

        if(!shaders[1].loadFromFile(provider.findPathToFile("tilemap.frag"), GL_FRAGMENT_SHADER))
            return false;

        if( ! m_landscape.shaderProgram.link(shaders) )
            return false;
    }

    if(m_tilemap.loadFromFile(provider.findPathToFile(std::string(info))))
    {
        auto vertices = m_tilemap.getVertices();
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        auto indices = m_tilemap.getIndices();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size_bytes()), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float4 };
        VertexArrayObject::createVertexInputState(vao[0], vbo[0], attributes);
        
        glBindVertexArray(vao[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_landscape.texture = landscapeTexture.handle;
        m_landscape.vao = vao[0];
        m_landscape.count = indices.size();

        createSystems();
        m_isLoaded = true;
    }

    return m_isLoaded;
}


void Mission::update(float dt) noexcept
{
    alignas(16) mat4 MVP;
    alignas(16) mat4 modelView;
    alignas(16) mat4 result;

    auto& camera = m_game->camera;
    camera.getModelViewProjectionMatrix(MVP);

    m_landscape.transform.calculate(modelView);
    glmc_mat4_mul(MVP, modelView, result);
    m_game->updateUniformBuffer(result);

    if(m_isLoaded)
    {
        for(auto system : m_systems)
            system(this, dt);
    }
}


void Mission::draw() noexcept
{
    m_landscape.shaderProgram(true);
    glBindTextureUnit(0, m_landscape.texture);
    glBindVertexArray(m_landscape.vao);
    glDrawElements(GL_TRIANGLES, m_landscape.count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glBindTextureUnit(0, 0);
    m_landscape.shaderProgram(false);
}


void Mission::resize(int width, int height) noexcept
{

}


void Mission::press(int key) noexcept
{
    vec2 movement = { 0, 0 };

    if(key == GLFW_KEY_A)
        movement[0] = 10.f;

    if(key == GLFW_KEY_D)
        movement[0] = -10.f;

    if(key == GLFW_KEY_W)
        movement[1] = 10.f;

    if(key == GLFW_KEY_S)
        movement[1] = -10.f;

    m_landscape.transform.move(movement);
}


void Mission::click(int button) noexcept
{

}


void Mission::setCursorPosition(float x, float y) noexcept
{

}


void Mission::createSystems() noexcept
{

}