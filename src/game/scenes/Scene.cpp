#include <cassert>

#include <cglm/call/cam.h>

#include "game/DuneII.hpp"
#include "game/scenes/Scene.hpp"


Scene::Scene(DuneII* game, const Scene::Type type) noexcept:
    m_game(game),
    m_isLoaded(false),
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


void Scene::draw() noexcept
{

}


void Scene::resize(int width, int height) noexcept
{

}


bool Scene::isLoaded() const noexcept
{
    return m_isLoaded;
}


Scene::Type Scene::getType() const noexcept
{
    return m_type;
}


void Scene::setSpriteSizeInPixels(const mesh::Sprite& sprite, vec2 newSize, Transform2D& transform) noexcept
{
    assert(sprite.width > 0);
    assert(sprite.height > 0);

    float dx = newSize[0] / sprite.width;
    float dy = newSize[1] / sprite.height;
    transform.setScale(dx, dy);
}