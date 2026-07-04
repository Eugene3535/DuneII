#include <cassert>

#include <cglm/call/cam.h>

#include "graphics/sprites/SpriteManager.hpp"
#include "application/game/Game.hpp"
#include "scenes/Scene.hpp"



Scene::Scene(Game* game, const Scene::Type type) noexcept:
    m_game(game),
    m_isLoaded(false),
    m_size{},
    m_cursor{},
    m_mouse{},
    m_type(type)
{

}


Scene::~Scene() = default;


bool Scene::load(std::string_view info) noexcept
{
    return false;
}


void Scene::update(float dt) noexcept
{

}


void Scene::draw(const mat4s& projection) noexcept
{

}


void Scene::resize(int width, int height) noexcept
{
    m_size = { width, height };
}


void Scene::setCursor(float x, float y) noexcept
{
    m_cursor = { x, y };
}


void Scene::setMouse(int button, int action, int mods) noexcept
{
    m_mouse.button = button;
    m_mouse.action = action;
    m_mouse.mods = mods;
}


void Scene::setKeyboard(int key, int scancode, int action, int mods) noexcept
{
    m_keyboard.key = key;
    m_keyboard.scancode = scancode;
    m_keyboard.action = action;
    m_keyboard.mods = mods;
}


bool Scene::isLoaded() const noexcept
{
    return m_isLoaded;
}


Scene::Type Scene::getType() const noexcept
{
    return m_type;
}


void Scene::setSpriteSizeInPixels(const Sprite2D& sprite, const vec2s newSize, Transform2D& transform) noexcept
{
    assert(sprite.width > 0);
    assert(sprite.height > 0);

    float dx = newSize.x / sprite.width;
    float dy = newSize.y / sprite.height;
    transform.setScale(dx, dy);
}