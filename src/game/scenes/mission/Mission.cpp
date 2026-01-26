#include <cstring>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "cglm/struct/affine-mat.h"

#include "resources/files/FileProvider.hpp"
#include "resources/gl_interfaces/texture/Texture.hpp"
#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/mission/Mission.hpp"


#define CAMERA_VELOCITY 600
#define SCREEN_MARGIN 150


Mission::Mission(DuneII* game) noexcept:
    Scene(game, Scene::MISSION),
    m_builder(m_registry, m_tileMask),
    m_hud(m_builder, m_transform),
    m_menu(game->getWindowsSize())
{
    memset(&m_landscape, 0, sizeof(m_landscape)); 
    memset(&m_buildings, 0, sizeof(m_buildings));

    m_ui.texture = 0;
    m_ui.program = 0;
    m_ui.clickTimer = 0.f;
}


Mission::~Mission()
{
    if (m_isLoaded)
    {
        GLuint textures[]            = { m_landscape.texture, m_buildings.texture, m_ui.texture };
        GLuint vertexArrayObjects[]  = { m_landscape.vao, m_buildings.vao         };
        GLuint vertexBufferObjects[] = { m_landscape.vbo[0], m_landscape.vbo[1]   };

        glDeleteTextures(std::size(textures), textures);
        glDeleteVertexArrays(std::size(vertexArrayObjects), vertexArrayObjects);
        glDeleteBuffers(std::size(vertexBufferObjects), vertexBufferObjects);
    }
}


bool Mission::load(std::string_view info) noexcept
{
    if(m_isLoaded)
        return true;

    if(!initLandscape())
        return false;

    if(!initHUD())
        return false;

    uint32_t frameProgram = m_game->getShaderProgram("color_outline");
    uint32_t previewProgram = m_game->getShaderProgram("sprite");

    if( ! (frameProgram && previewProgram) )
        return false;

    m_menu.init(frameProgram, previewProgram);

    if(m_tilemap.loadFromFile(FileProvider::findPathToFile(std::string(info))))
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

        m_landscape.vao = m_landscape.vao;
        m_landscape.count = indices.size();

        glGenTextures(1, &m_buildings.texture);
        glGenVertexArrays(1, &m_buildings.vao);

        VertexArrayObject::createVertexInputState(m_buildings.vao, m_builder.getVertexBuffer(), attributes);

        Texture buildingTexture = {.handle = m_buildings.texture };

        if(!buildingTexture.loadFromFile(FileProvider::findPathToFile(STRUCTURES_PNG)))
            return false;

        m_tileMask    = m_tilemap.getTileMask();
        auto mapSize  = m_tilemap.getMapSize();
        auto tileSize = m_tilemap.getTileSize();

        if(!m_builder.loadFromTileMap(m_tilemap, buildingTexture.handle))
            return false;
    }

    createSystems();

    return m_isLoaded;
}


void Mission::update(float dt) noexcept
{
    if (m_isLoaded)
        for(auto system : m_systems)
            system(this, dt);
}


void Mission::draw() noexcept
{
    auto& camera = m_game->camera;

    alignas(16) mat4s uniformMatrix = camera.getModelViewProjectionMatrix();
    alignas(16) mat4s modelView     = m_transform.getMatrix();
    alignas(16) mat4s result        = glms_mul(uniformMatrix, modelView);

    camera.updateUniformBuffer(result.raw);

    {// Landscape
        glUseProgram(m_landscape.program);
        glBindTextureUnit(0, m_landscape.texture);
        glBindVertexArray(m_landscape.vao);
        glDrawElements(GL_TRIANGLES, m_landscape.count, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glBindTextureUnit(0, 0);
    }

    {// Structures
        glBindTextureUnit(0, m_buildings.texture);
        glBindVertexArray(m_buildings.vao);

        auto view = m_registry.view<const StructureInfo>();

        view.each([](const StructureInfo& building) 
        {
            glDrawArrays(GL_TRIANGLE_FAN, building.frame, 4);
        });

        glBindVertexArray(0);
        glBindTextureUnit(0, 0);
    }

    {// HUD
        if(m_hud.isSelectionEnabled())
        {
            glUseProgram(m_ui.program);
            m_hud.drawSelection();
            glUseProgram(m_landscape.program);
        }
 
        modelView = m_hud.getCursorTransform().getMatrix();
        result = glms_mul(uniformMatrix, modelView);
        camera.updateUniformBuffer(result.raw);

        m_sprites.bind(true);
        m_hud.drawCursor();
        m_sprites.bind(false);
    }

    {// Test construction menu
        if(m_menu.isEnabled())
        {
            modelView = m_menu.getTransform().getMatrix();
            result = glms_mul(uniformMatrix, modelView);
            camera.updateUniformBuffer(result.raw);
            m_menu.draw();
        }
    }
}


bool Mission::initLandscape() noexcept
{
    glGenTextures(1, &m_landscape.texture);
    glGenBuffers(2, m_landscape.vbo);
    glGenVertexArrays(1, &m_landscape.vao);

    Texture landscapeTexture = {.handle = m_landscape.texture };

    if(!landscapeTexture.loadFromFile(FileProvider::findPathToFile(LANDSCAPE_PNG)))
        return false;

    m_landscape.texture = landscapeTexture.handle;

    if(m_landscape.program = m_game->getShaderProgram("tilemap"); m_landscape.program == 0)
        return false;

    return true;
}


bool Mission::initHUD() noexcept
{
    glGenTextures(1, &m_ui.texture);
    Texture crosshairTexture = {.handle = m_ui.texture };

    if(!crosshairTexture.loadFromFile(FileProvider::findPathToFile(CROSSHAIRS_TILESHEET_PNG)))
        return false;

    m_sprites.loadSpriteSheet(FileProvider::findPathToFile(CURSOR_FRAME_XML), crosshairTexture);
    auto crosshairReleased = m_sprites.getSprite("Released");
    auto crosshairCaptured = m_sprites.getSprite("Captured");

    if(! (crosshairReleased && crosshairCaptured) )
        return false;

    std::array<Sprite, 2> crosshairs = 
    {
        crosshairReleased.value(),
        crosshairCaptured.value()
    };

    if(m_ui.program = m_game->getShaderProgram("selection"); m_ui.program == 0)
        return false;

    auto showMenuForEntityCallback = [this](const entt::entity e) -> void
    {
        m_menu.enable();
    };

    m_hud.init(crosshairs, showMenuForEntityCallback);

    return true;
}


void Mission::createSystems() noexcept
{
//  Viewport Controller
    m_systems.emplace_back([](Mission* mission, float dt)
    {
        const auto game     = mission->m_game;
        const auto cursor   = game->getCursorPosition();
        const auto viewSize = game->getWindowsSize();
        const auto mapSize  = glms_ivec2_mul(mission->m_tilemap.getMapSize(), mission->m_tilemap.getTileSize());

        const bool is_near_the_left_edge   = (cursor.x > 0 && cursor.x < SCREEN_MARGIN);
        const bool is_near_the_top_edge    = (cursor.y > 0 && cursor.y < SCREEN_MARGIN);
        const bool is_near_the_right_edge  = (cursor.x > (viewSize.x - SCREEN_MARGIN) && cursor.x < viewSize.x);
        const bool is_near_the_bottom_edge = (cursor.y > (viewSize.y - SCREEN_MARGIN) && cursor.y < viewSize.y);

        const float velocity = dt * CAMERA_VELOCITY;
        vec2s scenePosition = mission->m_transform.getPosition();

        if(is_near_the_left_edge)
            scenePosition.x += velocity;
        
        if(is_near_the_top_edge)
            scenePosition.y += velocity;
        
        if(is_near_the_right_edge)
            scenePosition.x -= velocity;
        
        if(is_near_the_bottom_edge)
            scenePosition.y -= velocity;

        if(scenePosition.x > 0)                        scenePosition.x = 0;
        if(scenePosition.y > 0)                        scenePosition.y = 0;
        if(scenePosition.x < (viewSize.x - mapSize.x)) scenePosition.x = viewSize.x - mapSize.x;
        if(scenePosition.y < (viewSize.y - mapSize.y)) scenePosition.y = viewSize.y - mapSize.y;

        mission->m_transform.setPosition(scenePosition);
    });

//  HUD Controller
    m_systems.emplace_back([](Mission* mission, float dt)
    {
        const auto game   = mission->m_game;
        const auto cursor = game->getCursorPosition();

        mission->m_ui.clickTimer += dt;

        if(mission->m_ui.clickTimer > 0.1f)
        {
            mission->m_ui.clickTimer = 0.f;

            const bool isMouseButtoLeftPressed = game->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
            const bool isMouseButtoRightPressed = game->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);

            if(isMouseButtoLeftPressed)
                mission->m_hud.select();

            if(isMouseButtoRightPressed)
                mission->m_hud.release();
        }
 
        mission->m_hud.update(cursor, dt);

        if(mission->m_menu.isEnabled())
        {
            mission->m_menu.update();

            if(game->isKeyPressed(GLFW_KEY_SPACE))
                mission->m_menu.disable();
        }
    });

    m_isLoaded = true;
}