#include <SFML/Graphics/Sprite.hpp>

#include "common/GraphicsUtils.hpp"

void GraphicsUtils::setSpriteSize(sf::Sprite& sprite, float width, float height) noexcept
{
	if (auto& rect = sprite.getTextureRect(); rect.width > 0 && rect.height > 0)
	{
		float dx = width / rect.width;
		float dy = height / rect.height;
		sprite.setScale(dx, dy);
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

	int width  = static_cast<int>(rect.width * current_scale.x);
	int height = static_cast<int>(rect.height * current_scale.y);

	return sf::Vector2i(width, height);
}