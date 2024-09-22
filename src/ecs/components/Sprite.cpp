#include "ecs/components/Sprite.hpp"

BEGIN_NAMESPACE_ECS

Sprite::Sprite() noexcept:
    sf::Sprite()
{

}

Sprite::Sprite(const sf::Texture* texture) noexcept:
    sf::Sprite(*texture)
{

}

Sprite::Sprite(const sf::Texture* texture, const sf::IntRect& rectangle) noexcept:
    sf::Sprite(*texture, rectangle)
{

}

void Sprite::setSize(int32_t width, int32_t height) noexcept
{
	if (const auto& rect = getTextureRect(); (rect.width && rect.height))
	{
		float dx = static_cast<float>(width  / rect.width);
		float dy = static_cast<float>(height / rect.height);
		setScale(dx, dy);
	}
}

void Sprite::setSize(const sf::Vector2i& size) noexcept
{
	setSize(size.x, size.y);
}

sf::Vector2i Sprite::getSize() const noexcept
{
	const auto& rect = getTextureRect();
	const auto& scale = getScale();

	return { static_cast<int32_t>(rect.width * scale.x), static_cast<int32_t>(rect.height * scale.y) };
}

END_NAMESPACE_ECS