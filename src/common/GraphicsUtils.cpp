#include <SFML/Graphics/Sprite.hpp>

#include "common/GraphicsUtils.hpp"

void GraphicsUtils::setSpriteSize(sf::Sprite& sprite, float width, float height) noexcept
{
	if (auto& rect = sprite.getTextureRect(); rect.size.x > 0 && rect.size.y > 0)
	{
		float dx = width / rect.size.x;
		float dy = height / rect.size.y;
		sprite.setScale({dx, dy});
	}
}

void GraphicsUtils::setSpriteSize(sf::Sprite& sprite, const sf::Vector2i& size) noexcept
{
	setSpriteSize(sprite, size.x, size.y);
}

sf::Vector2i GraphicsUtils::getSpriteSize(const sf::Sprite& sprite) noexcept
{
	const auto& rect = sprite.getTextureRect();
	const sf::Vector2f& current_scale = sprite.getScale();

	int width  = static_cast<int>(rect.size.x * current_scale.x);
	int height = static_cast<int>(rect.size.y * current_scale.y);

	return sf::Vector2i(width, height);
}