#include "game/Game.hpp"
#include "scenes/base/Scene.hpp"

Scene::Scene(Game& game) noexcept:
    m_game(game),
    m_isLoaded(false)
{

}

Scene::~Scene()
{

}

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