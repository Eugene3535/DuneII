#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include <SFML/Graphics/Sprite.hpp>

#include "ecs/common/Component.hpp"

class SpriteComponent: 
    public ecs::Component<SpriteComponent>,
    public sf::Sprite
{
public:
    SpriteComponent() noexcept;
    SpriteComponent(const sf::Texture* texture) noexcept;
    SpriteComponent(const sf::Texture* texture, const sf::IntRect& rectangle) noexcept;
    ~SpriteComponent();

//  Allows to set and get size of sprite in pixels(instead of scale)
	void setSize(int32_t width, int32_t height) noexcept;
	void setSize(const sf::Vector2i& size) noexcept;

	sf::Vector2i getSize() const noexcept;
};



#endif // !SPRITE_COMPONENT_HPP