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


void Scene::update(const sf::Time dt) noexcept
{

}


void Scene::resize(const sf::Vector2f& size) noexcept
{
    m_view.setCenter(size * 0.5f);
    m_view.setSize(size);
}


bool Scene::isLoaded() const noexcept
{
    return m_isLoaded;
}


std::pair<Scene::Type, bool> Scene::getStatus() const noexcept
{
    // if(game_state.isSceneNeedToBeChanged)
    //         return { game_state.nextScene, true };

    return { Scene::Type::NONE, false };
}


const sf::View& Scene::getView() const noexcept
{
    return m_view;
}