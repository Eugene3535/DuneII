#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "resources/files/Shader.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/mission/Mission.hpp"


Mission::Mission(DuneII* game) noexcept:
    Scene(game),
    m_builder(m_registry, m_tileMask)
{
    memset(&m_landscape, 0, sizeof(m_landscape)); 
    memset(&m_buildings, 0, sizeof(m_buildings));
}


Mission::~Mission()
{
    if (m_isLoaded)
    {
        GLuint textures[] = { m_landscape.texture, m_buildings.texture };
        GLuint vertexArrayObjects[] = { m_landscape.vao, m_buildings.vao };
        GLuint vertexBufferObjects[] = { m_landscape.vbo[0], m_landscape.vbo[1] };

        glDeleteTextures(std::size(textures), textures);
        glDeleteVertexArrays(std::size(vertexArrayObjects), vertexArrayObjects);
        glDeleteBuffers(std::size(vertexBufferObjects), vertexBufferObjects);
    }
}


bool Mission::load(std::string_view info) noexcept
{
    if(m_isLoaded)
        return true;
  
    auto& provider = m_game->fileProvider;

    glGenTextures(1, &m_landscape.texture);
    glGenBuffers(2, m_landscape.vbo);
    glGenVertexArrays(1, &m_landscape.vao);

    Texture landscapeTexture = {.handle = m_landscape.texture };

    if(!landscapeTexture.loadFromFile(provider.findPathToFile(LANDSCAPE_PNG)))
        return false;

    {
        std::array<Shader, 2> shaders;

        if(!shaders[0].loadFromFile(provider.findPathToFile("tilemap.vert"), GL_VERTEX_SHADER))
            return false;

        if(!shaders[1].loadFromFile(provider.findPathToFile("tilemap.frag"), GL_FRAGMENT_SHADER))
            return false;

        if( ! m_landscape.program.link(shaders) )
            return false;
    }

    if(m_tilemap.loadFromFile(provider.findPathToFile(std::string(info))))
    {
        auto vertices = m_tilemap.getVertices();
        glBindBuffer(GL_ARRAY_BUFFER, m_landscape.vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        auto indices = m_tilemap.getIndices();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_landscape.vbo[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size_bytes()), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        const std::array<VertexBufferLayout::Attribute, 1> attributes{ VertexBufferLayout::Attribute::Float4 };
        VertexArrayObject::createVertexInputState(m_landscape.vao, m_landscape.vbo[0], attributes);
        
        glBindVertexArray(m_landscape.vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_landscape.vbo[1]);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_landscape.texture = landscapeTexture.handle;
        m_landscape.vao = m_landscape.vao;
        m_landscape.count = indices.size();

        glGenTextures(1, &m_buildings.texture);
        glGenVertexArrays(1, &m_buildings.vao);

        VertexArrayObject::createVertexInputState(m_buildings.vao, m_builder.getVertexBuffer(), attributes);

        Texture buildingTexture = {.handle = m_buildings.texture };

        if(!buildingTexture.loadFromFile(provider.findPathToFile(STRUCTURES_PNG)))
            return false;

        m_tileMask    = m_tilemap.getTileMask();
        auto mapSize  = m_tilemap.getMapSize();
        auto tileSize = m_tilemap.getTileSize();

        if(!m_builder.loadFromTileMap(m_tilemap, buildingTexture.handle))
            return false;

        createSystems();

        m_isLoaded = true;
    }

    return m_isLoaded;
}


void Mission::update(float dt) noexcept
{
    if (!m_isLoaded)
        return;

    {
        vec2s movement = { 0, 0 };

        if (m_game->isKeyPressed(GLFW_KEY_A))
            movement.x = 10.f;

        if (m_game->isKeyPressed(GLFW_KEY_D))
            movement.x = -10.f;

        if (m_game->isKeyPressed(GLFW_KEY_W))
            movement.y = 10.f;

        if (m_game->isKeyPressed(GLFW_KEY_S))
            movement.y = -10.f;


        m_transform.move(movement);
    }

    alignas(16) mat4 MVP;
    alignas(16) mat4 modelView;
    alignas(16) mat4 result;

    auto& camera = m_game->camera;
    camera.getModelViewProjectionMatrix(MVP);

    m_transform.calculate(modelView);
    glmc_mat4_mul(MVP, modelView, result);
    camera.updateUniformBuffer(result);
}


void Mission::draw() noexcept
{
    m_landscape.program(true);

    glBindTextureUnit(0, m_landscape.texture);
    glBindVertexArray(m_landscape.vao);
    glDrawElements(GL_TRIANGLES, m_landscape.count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glBindTextureUnit(0, 0);

    glBindTextureUnit(0, m_buildings.texture);
    glBindVertexArray(m_buildings.vao);

    auto view = m_registry.view<const Structure>();

    view.each([](const Structure& building) 
    {
        glDrawArrays(GL_TRIANGLE_FAN, building.frame, 4);
    });

    glBindVertexArray(0);
    glBindTextureUnit(0, 0);

    m_landscape.program(false);
}


void Mission::resize(int width, int height) noexcept
{

}


void Mission::createSystems() noexcept
{

}