#include <cassert>

#include <cglm/call/cam.h>

#include "graphics/sprites/SpriteManager.hpp"
#include "app/game/Game.hpp"
#include "scenes/Scene.hpp"



Scene::Scene(Game* game) noexcept:
    m_game(game),
    m_isLoaded(false)
{

}


Scene::~Scene() = default;


bool Scene::isLoaded() const noexcept
{
    return m_isLoaded;
}


void Scene::setSpriteSizeInPixels(const Sprite2D& sprite, const vec2s newSize, Transform2D& transform) noexcept
{
    assert(sprite.width > 0);
    assert(sprite.height > 0);

    float dx = newSize.x / sprite.width;
    float dy = newSize.y / sprite.height;
    transform.setScale(dx, dy);
}