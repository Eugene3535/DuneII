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


void Scene::resize(const glm::ivec2& size) noexcept
{
    
}


bool Scene::isLoaded() const noexcept
{
    return m_isLoaded;
}