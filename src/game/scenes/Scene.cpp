#include <cassert>

#include "graphics/Mesh.hpp"
#include "graphics/Transform2D.hpp"
#include "game/DuneII.hpp"
#include "game/scenes/Scene.hpp"


Scene::Scene(DuneII* game) noexcept:
    m_game(game),
    m_isLoaded(false)
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


void Scene::resize(const glm::ivec2& size) noexcept
{
    
}


bool Scene::isLoaded() const noexcept
{
    return m_isLoaded;
}


void Scene::setSpriteSizeInPixels(const Sprite& sprite, const glm::vec2& newSize, Transform2D& transform) noexcept
{
    assert(sprite.width > 0);
    assert(sprite.height > 0);

    float dx = newSize.x / sprite.width;
    float dy = newSize.y / sprite.height;
    transform.setScale(dx, dy);
}