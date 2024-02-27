#include "scenes/Scene.hpp"

Scene::Scene() noexcept:
    m_loadingProgress(0),
    m_isLoaded(false)
{
}

Scene::~Scene()
{
}

void Scene::setCursorPosition(const sf::Vector2i& point) noexcept
{
    m_cursorPosition = point;
}

sf::View& Scene::getViewport() noexcept
{
    return m_viewport;
}

std::int32_t Scene::getLoadingProgress() const noexcept
{
    return m_loadingProgress;
}

bool Scene::isLoaded() const noexcept
{
    return m_isLoaded;
}

void Scene::setSpriteSize(sf::Sprite& sprite, int width, int height)
{
    const auto& rect = sprite.getTextureRect();

	if (rect.width && rect.height)
	{
		float dx = width / std::abs(rect.width);
		float dy = height / std::abs(rect.height);
		sprite.setScale(dx, dy);
	}
}

void Scene::setSpriteSize(sf::Sprite& sprite, const sf::Vector2i& size)
{
	setSpriteSize(sprite, size.x, size.y);
}

sf::Vector2i Scene::getSpriteSize(const sf::Sprite& sprite)
{
	const auto& rect  = sprite.getTextureRect();
	const auto& scale = sprite.getScale();

	return { static_cast<std::int32_t>(rect.width * scale.x), static_cast<std::int32_t>(rect.height * scale.y) };
}