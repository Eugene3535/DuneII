#ifndef BOUNDS_COMPONENT_HPP
#define BOUNDS_COMPONENT_HPP

#include <SFML/Graphics/Rect.hpp>

#include "ecs/common/Component.hpp"

BEGIN_NAMESPACE_ECS

class Bounds: 
    public ecs::Component<Bounds>,
    public sf::IntRect
{
public:
    Bounds() noexcept;
    Bounds(int32_t rectLeft, int32_t rectTop, int32_t rectWidth, int32_t rectHeight) noexcept;
    Bounds(sf::Vector2i& position, sf::Vector2i& size) noexcept;
};

END_NAMESPACE_ECS

#endif // !SPRITE_COMPONENT_HPP