#include "game/DuneII.hpp"
#include "scenes/Scene.hpp"


Scene::Scene(DuneII* game) noexcept:
    m_game(game),
    m_isLoaded(false)
{

}


Scene::~Scene() = default;


bool Scene::load(const std::string& info) noexcept
{
    return false;
}


void Scene::update(sf::Time dt) noexcept
{

}


bool Scene::isLoaded() const noexcept
{
    return m_isLoaded;
}