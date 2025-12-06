#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "game/DuneII.hpp"
#include "game/scenes/mission/Mission.hpp"


Mission::Mission(DuneII* game) noexcept:
    Scene(game)
{

}


Mission::~Mission()
{

}


bool Mission::load(std::string_view info) noexcept
{
    if(m_isLoaded)
        return true;
  
    auto& provider = m_game->fileProvider;
    auto& glResources = m_game->glResources;

    auto vboHandles     = glResources.create<GLBuffer, 2>();
    auto vaoHandles     = glResources.create<VertexArrayObject, 1>();
    auto textureHandles = glResources.create<Texture, 1>();
    
    Texture landscapeTexture = {.handle = textureHandles[0] };

    if(!landscapeTexture.loadFromFile(provider.findPathToFile(LANDSCAPE_PNG)))
        return false;

    if(m_landscape.shaderProgram = glResources.getShaderProgram("sprite"); m_landscape.shaderProgram == 0)
        return false;

    if(m_tilemap.loadFromFile(provider.findPathToFile(std::string(info))))
    {
        auto vertices = m_tilemap.getVertices();
        auto indices = m_tilemap.getIndices();

        GLBuffer vbo(vboHandles[0], GL_ARRAY_BUFFER);
        GLBuffer ebo(vboHandles[1], GL_ELEMENT_ARRAY_BUFFER);
        vbo.create(sizeof(vec4s), vertices.size(), vertices.data(), GL_STATIC_DRAW);
        ebo.create(sizeof(GLuint), indices.size(), indices.data(), GL_STATIC_DRAW);

        VertexArrayObject vao(vaoHandles[0]);
        const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float4 };
        vao.addVertexBuffer(vbo, attributes);
        vao.setElementBuffer(ebo);

        m_landscape.texture = landscapeTexture.handle;
        m_landscape.vao = vao.getHandle();
        m_landscape.count = vao.getIndexCount();

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
    glUseProgram(m_landscape.shaderProgram);
    glBindTextureUnit(0, m_landscape.texture);
    glBindVertexArray(m_landscape.vao);
    glDrawElements(GL_TRIANGLES, m_landscape.count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glBindTextureUnit(0, 0);
    glUseProgram(0);
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