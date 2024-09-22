#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include <SFML/Graphics/Sprite.hpp>

#include "ecs/common/Component.hpp"

BEGIN_NAMESPACE_ECS

class Sprite: 
    public ecs::Component<Sprite>,
    public sf::Sprite
{
public:
    Sprite() noexcept;
    Sprite(const sf::Texture* texture) noexcept;
    Sprite(const sf::Texture* texture, const sf::IntRect& rectangle) noexcept;

//  Allows to set and get size of sprite in pixels(instead of scale)
	void setSize(int32_t width, int32_t height) noexcept;
	void setSize(const sf::Vector2i& size) noexcept;

	sf::Vector2i getSize() const noexcept;
};

END_NAMESPACE_ECS

#endif // !SPRITE_COMPONENT_HPP