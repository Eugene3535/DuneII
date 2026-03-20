#include "game/DuneII.hpp"
#include "game/scenes/Scene.hpp"


Scene::Scene(DuneII* game, const Scene::Type type) noexcept:
    m_game(game),
    m_isLoaded(false),
    m_type(type)
{

}


Scene::~Scene() = default;


bool Scene::load(std::string_view data) noexcept
{
    return false;
}


void Scene::update(sf::Time dt) noexcept
{

}


void Scene::resize(sf::Vector2u size) noexcept
{
    m_view.setCenter(sf::Vector2f(size) * 0.5f);
    m_view.setSize(sf::Vector2f(size));
}


bool Scene::isLoaded() const noexcept
{
    return m_isLoaded;
}


Scene::Type Scene::getType() const noexcept
{
    return m_type;
}


void Scene::setSpriteSizeInPixels(sf::Sprite& sprite, sf::Vector2f size) noexcept
{
    if (const auto& rect = sprite.getTextureRect(); rect.size.x > 0 && rect.size.y > 0)
    {
        float dx = size.x / static_cast<float>(rect.size.x);
        float dy = size.y / static_cast<float>(rect.size.y);
        sprite.setScale({dx, dy});
    }
}


void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}