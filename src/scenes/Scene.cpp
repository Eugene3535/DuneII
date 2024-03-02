#include "game/Game.hpp"
#include "scenes/Scene.hpp"

Scene::Scene(Game* game) noexcept:
    m_game(game),
    m_isLoaded(false)
{
}

Scene::~Scene()
{
}

bool Scene::isLoaded() const noexcept
{
    return m_isLoaded;
}